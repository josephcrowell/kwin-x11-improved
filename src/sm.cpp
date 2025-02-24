/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 1999, 2000 Matthias Ettrich <ettrich@kde.org>
    SPDX-FileCopyrightText: 2003 Lubos Lunak <l.lunak@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "sm.h"

#include <cstdlib>
#include <kconfig.h>
#include <pwd.h>
#include <unistd.h>

#include "virtualdesktops.h"
#include "wayland_server.h"
#include "workspace.h"
#include "xdgshellwindow.h"
#include <QDebug>
#if KWIN_BUILD_X11
#include "x11window.h"
#endif

#include <QSessionManager>
#if KWIN_BUILD_NOTIFICATIONS
#include <KLocalizedString>
#include <KNotification>
#include <KService>
#endif

#include "sessionadaptor.h"

using namespace Qt::StringLiterals;

namespace KWin
{

static KConfig *sessionConfig(QString id, QString key)
{
    static KConfig *config = nullptr;
    static QString lastId;
    static QString lastKey;
    static QString pattern = QString(QLatin1String("session/%1_%2_%3")).arg(qApp->applicationName());
    if (id != lastId || key != lastKey) {
        delete config;
        config = nullptr;
    }
    lastId = id;
    lastKey = key;
    if (!config) {
        config = new KConfig(pattern.arg(id, key), KConfig::SimpleConfig);
    }
    return config;
}

static const char *const window_type_names[] = {
    "Unknown", "Normal", "Desktop", "Dock", "Toolbar", "Menu", "Dialog",
    "Override", "TopMenu", "Utility", "Splash"};
// change also the two functions below when adding new entries

static const char *windowTypeToTxt(WindowType type)
{
    if (type >= WindowType::Unknown && type <= WindowType::Splash) {
        return window_type_names[int(type) + 1]; // +1 (unknown==-1)
    }
    if (type == WindowType::Undefined) { // undefined (not really part of WindowType)
        return "Undefined";
    }
    qFatal("Unknown Window Type");
    return nullptr;
}

static WindowType txtToWindowType(const char *txt)
{
    for (int i = int(WindowType::Unknown); i <= int(WindowType::Splash); ++i) {
        if (qstrcmp(txt, window_type_names[i + 1]) == 0) { // +1
            return static_cast<WindowType>(i);
        }
    }
    return WindowType::Undefined;
}

/**
 * Stores the current session in the config file
 *
 * @see loadSessionInfo
 */
void SessionManager::storeSession(const QString &sessionName, SMSavePhase phase)
{
    qCDebug(KWIN_CORE) << "storing session" << sessionName << "in phase" << phase;
    KConfig *config = sessionConfig(sessionName, QString());

    KConfigGroup cg(config, QStringLiteral("Session"));
    int count = 0;
    int active_client = -1;

#if KWIN_BUILD_X11
    const QList<Window *> windows = workspace()->windows();
    for (auto it = windows.begin(); it != windows.end(); ++it) {
        X11Window *c = qobject_cast<X11Window *>(*it);
        if (!c || c->isUnmanaged()) {
            continue;
        }
        if (c->windowType() > WindowType::Splash) {
            // window types outside this are not tooltips/menus/OSDs
            // typically these will be unmanaged and not in this list anyway, but that is not enforced
            continue;
        }
        QByteArray sessionId = c->sessionId();
        QString wmCommand = c->wmCommand();
        if (sessionId.isEmpty()) {
            // remember also applications that are not XSMP capable
            // and use the obsolete WM_COMMAND / WM_SAVE_YOURSELF
            if (wmCommand.isEmpty()) {
                continue;
            }
        }
        count++;
        if (c->isActive()) {
            active_client = count;
        }
        if (phase == SMSavePhase2 || phase == SMSavePhase2Full) {
            storeClient(cg, count, c);
        }
    }
#endif
    if (phase == SMSavePhase0) {
        // it would be much simpler to save these values to the config file,
        // but both Qt and KDE treat phase1 and phase2 separately,
        // which results in different sessionkey and different config file :(
        m_sessionActiveClient = active_client;
        m_sessionDesktop = VirtualDesktopManager::self()->current();
    } else if (phase == SMSavePhase2) {
        cg.writeEntry("count", count);
        cg.writeEntry("active", m_sessionActiveClient);
        cg.writeEntry("desktop", m_sessionDesktop);
    } else { // SMSavePhase2Full
        cg.writeEntry("count", count);
        cg.writeEntry("active", m_sessionActiveClient);
    }
    config->sync(); // it previously did some "revert to defaults" stuff for phase1 I think
}

#if KWIN_BUILD_X11
void SessionManager::storeClient(KConfigGroup &cg, int num, X11Window *c)
{
    c->setSessionActivityOverride(false); // make sure we get the real values
    QString n = QString::number(num);
    cg.writeEntry(QLatin1String("sessionId") + n, c->sessionId().constData());
    cg.writeEntry(QLatin1String("windowRole") + n, c->windowRole());
    cg.writeEntry(QLatin1String("wmCommand") + n, c->wmCommand());
    cg.writeEntry(QLatin1String("resourceName") + n, c->resourceName());
    cg.writeEntry(QLatin1String("resourceClass") + n, c->resourceClass());
    cg.writeEntry(QLatin1String("geometry") + n, QRectF(c->calculateGravitation(true), c->clientSize()).toRect()); // FRAME
    cg.writeEntry(QLatin1String("restore") + n, c->geometryRestore());
    cg.writeEntry(QLatin1String("fsrestore") + n, c->fullscreenGeometryRestore());
    cg.writeEntry(QLatin1String("maximize") + n, (int)c->maximizeMode());
    cg.writeEntry(QLatin1String("fullscreen") + n, (int)c->fullScreenMode());
    cg.writeEntry(QLatin1String("desktop") + n, c->desktopId());
    // the config entry is called "iconified" for back. comp. reasons
    // (kconf_update script for updating session files would be too complicated)
    cg.writeEntry(QLatin1String("iconified") + n, c->isMinimized());
    cg.writeEntry(QLatin1String("opacity") + n, c->opacity());
    // the config entry is called "sticky" for back. comp. reasons
    cg.writeEntry(QLatin1String("sticky") + n, c->isOnAllDesktops());
    cg.writeEntry(QLatin1String("shaded") + n, c->isShade());
    // the config entry is called "staysOnTop" for back. comp. reasons
    cg.writeEntry(QLatin1String("staysOnTop") + n, c->keepAbove());
    cg.writeEntry(QLatin1String("keepBelow") + n, c->keepBelow());
    cg.writeEntry(QLatin1String("skipTaskbar") + n, c->originalSkipTaskbar());
    cg.writeEntry(QLatin1String("skipPager") + n, c->skipPager());
    cg.writeEntry(QLatin1String("skipSwitcher") + n, c->skipSwitcher());
    // not really just set by user, but name kept for back. comp. reasons
    cg.writeEntry(QLatin1String("userNoBorder") + n, c->userNoBorder());
    cg.writeEntry(QLatin1String("windowType") + n, windowTypeToTxt(c->windowType()));
    cg.writeEntry(QLatin1String("shortcut") + n, c->shortcut().toString());
    cg.writeEntry(QLatin1String("stackingOrder") + n, workspace()->unconstrainedStackingOrder().indexOf(c));
    cg.writeEntry(QLatin1String("activities") + n, c->activities());
}
#endif

#if KWIN_BUILD_X11
void SessionManager::storeSubSession(const QString &name, QSet<QByteArray> sessionIds)
{
    // TODO clear it first
    KConfigGroup cg(KSharedConfig::openConfig(), QLatin1String("SubSession: ") + name);
    int count = 0;
    int active_client = -1;
    const QList<Window *> windows = workspace()->windows();

    for (auto it = windows.begin(); it != windows.end(); ++it) {
        X11Window *c = qobject_cast<X11Window *>(*it);
        if (!c || c->isUnmanaged()) {
            continue;
        }
        if (c->windowType() > WindowType::Splash) {
            continue;
        }
        QByteArray sessionId = c->sessionId();
        QString wmCommand = c->wmCommand();
        if (sessionId.isEmpty()) {
            // remember also applications that are not XSMP capable
            // and use the obsolete WM_COMMAND / WM_SAVE_YOURSELF
            if (wmCommand.isEmpty()) {
                continue;
            }
        }
        if (!sessionIds.contains(sessionId)) {
            continue;
        }

        qCDebug(KWIN_CORE) << "storing" << sessionId;
        count++;
        if (c->isActive()) {
            active_client = count;
        }
        storeClient(cg, count, c);
    }
    cg.writeEntry("count", count);
    cg.writeEntry("active", active_client);
    // cg.writeEntry( "desktop", currentDesktop());
}
#endif

/**
 * Loads the session information from the config file.
 *
 * @see storeSession
 */
void SessionManager::loadSession(const QString &sessionName)
{
    session.clear();
    KConfigGroup cg(sessionConfig(sessionName, QString()), QStringLiteral("Session"));
    Q_EMIT loadSessionRequested(sessionName);
    addSessionInfo(cg);
}

void SessionManager::addSessionInfo(KConfigGroup &cg)
{
    int count = cg.readEntry("count", 0);
    int active_client = cg.readEntry("active", 0);
    for (int i = 1; i <= count; i++) {
        QString n = QString::number(i);
        SessionInfo *info = new SessionInfo;
        session.append(info);
        info->sessionId = cg.readEntry(QLatin1String("sessionId") + n, QString()).toLatin1();
        info->windowRole = cg.readEntry(QLatin1String("windowRole") + n, QString());
        info->wmCommand = cg.readEntry(QLatin1String("wmCommand") + n, QString()).toLatin1();
        info->resourceName = cg.readEntry(QLatin1String("resourceName") + n, QString());
        info->resourceClass = cg.readEntry(QLatin1String("resourceClass") + n, QString()).toLower();
        info->geometry = cg.readEntry(QLatin1String("geometry") + n, QRect());
        info->restore = cg.readEntry(QLatin1String("restore") + n, QRect());
        info->fsrestore = cg.readEntry(QLatin1String("fsrestore") + n, QRect());
        info->maximized = cg.readEntry(QLatin1String("maximize") + n, 0);
        info->fullscreen = cg.readEntry(QLatin1String("fullscreen") + n, 0);
        info->desktop = cg.readEntry(QLatin1String("desktop") + n, 0);
        info->minimized = cg.readEntry(QLatin1String("iconified") + n, false);
        info->opacity = cg.readEntry(QLatin1String("opacity") + n, 1.0);
        info->onAllDesktops = cg.readEntry(QLatin1String("sticky") + n, false);
        info->shaded = cg.readEntry(QLatin1String("shaded") + n, false);
        info->keepAbove = cg.readEntry(QLatin1String("staysOnTop") + n, false);
        info->keepBelow = cg.readEntry(QLatin1String("keepBelow") + n, false);
        info->skipTaskbar = cg.readEntry(QLatin1String("skipTaskbar") + n, false);
        info->skipPager = cg.readEntry(QLatin1String("skipPager") + n, false);
        info->skipSwitcher = cg.readEntry(QLatin1String("skipSwitcher") + n, false);
        info->noBorder = cg.readEntry(QLatin1String("userNoBorder") + n, false);
        info->windowType = txtToWindowType(cg.readEntry(QLatin1String("windowType") + n, QString()).toLatin1().constData());
        info->shortcut = cg.readEntry(QLatin1String("shortcut") + n, QString());
        info->active = (active_client == i);
        info->stackingOrder = cg.readEntry(QLatin1String("stackingOrder") + n, -1);
        info->activities = cg.readEntry(QLatin1String("activities") + n, QStringList());
    }
}

void SessionManager::loadSubSessionInfo(const QString &name)
{
    KConfigGroup cg(KSharedConfig::openConfig(), QLatin1String("SubSession: ") + name);
    addSessionInfo(cg);
}

#if KWIN_BUILD_X11
static bool sessionInfoWindowTypeMatch(X11Window *c, SessionInfo *info)
{
    if (int(info->windowType) == -2) {
        // undefined (not really part of NET::WindowType)
        return !c->isSpecialWindow();
    }
    return info->windowType == c->windowType();
}

/**
 * Returns a SessionInfo for client \a c. The returned session
 * info is removed from the storage. It's up to the caller to delete it.
 *
 * This function is called when a new window is mapped and must be managed.
 * We try to find a matching entry in the session.
 *
 * May return 0 if there's no session info for the client.
 */
SessionInfo *SessionManager::takeSessionInfo(X11Window *c)
{
    SessionInfo *realInfo = nullptr;
    QByteArray sessionId = c->sessionId();
    QString windowRole = c->windowRole();
    QString wmCommand = c->wmCommand();
    QString resourceName = c->resourceName();
    QString resourceClass = c->resourceClass();

    // First search ``session''
    if (!sessionId.isEmpty()) {
        // look for a real session managed client (algorithm suggested by ICCCM)
        for (SessionInfo *info : std::as_const(session)) {
            if (realInfo) {
                break;
            }
            if (info->sessionId == sessionId && sessionInfoWindowTypeMatch(c, info)) {
                if (!windowRole.isEmpty()) {
                    if (info->windowRole == windowRole) {
                        realInfo = info;
                        session.removeAll(info);
                    }
                } else {
                    if (info->windowRole.isEmpty()
                        && info->resourceName == resourceName
                        && info->resourceClass == resourceClass) {
                        realInfo = info;
                        session.removeAll(info);
                    }
                }
            }
        }
    } else {
        // look for a sessioninfo with matching features.
        for (SessionInfo *info : std::as_const(session)) {
            if (realInfo) {
                break;
            }
            if (info->resourceName == resourceName
                && info->resourceClass == resourceClass
                && sessionInfoWindowTypeMatch(c, info)) {
                if (wmCommand.isEmpty() || info->wmCommand == wmCommand) {
                    realInfo = info;
                    session.removeAll(info);
                }
            }
        }
    }
    return realInfo;
}
#endif

SessionManager::SessionManager(QObject *parent)
    : QObject(parent)
{
    new SessionAdaptor(this);
    QDBusConnection::sessionBus().registerObject(QStringLiteral("/Session"), this);
}

SessionManager::~SessionManager()
{
    qDeleteAll(session);
}

SessionState SessionManager::state() const
{
    return m_sessionState;
}

void SessionManager::setState(uint state)
{
    switch (state) {
    case 0:
        setState(SessionState::Saving);
        break;
    case 1:
        setState(SessionState::Quitting);
        break;
    default:
        setState(SessionState::Normal);
    }
}

// TODO should we rethink this now that we have dedicated start end end save methods?
void SessionManager::setState(SessionState state)
{
    if (state == m_sessionState) {
        return;
    }
    // If we're starting to save a session
    if (state == SessionState::Saving) {
        workspace()->rulebook()->setUpdatesDisabled(true);
    }
    // If we're ending a save session due to either completion or cancellation
    if (m_sessionState == SessionState::Saving) {
        workspace()->rulebook()->setUpdatesDisabled(false);
#if KWIN_BUILD_X11
        Workspace::self()->forEachClient([](X11Window *client) {
            client->setSessionActivityOverride(false);
        });
#endif
    }

    m_sessionState = state;
    Q_EMIT stateChanged();
}

void SessionManager::aboutToSaveSession(const QString &name)
{
    Q_EMIT prepareSessionSaveRequested(name);
    storeSession(name, SMSavePhase0);
}

void SessionManager::finishSaveSession(const QString &name)
{
    Q_EMIT finishSessionSaveRequested(name);
    storeSession(name, SMSavePhase2);
}

bool SessionManager::closeWaylandWindows()
{
    Q_ASSERT(calledFromDBus());
    if (!waylandServer()) {
        return true;
    }

    if (m_closingWindowsGuard) {
        sendErrorReply(QDBusError::Failed, u"Operation already in progress"_s);
        return false;
    }

    m_closingWindowsGuard = std::make_unique<QObject>();
    qCDebug(KWIN_CORE) << "Closing windows";

    auto dbusMessage = message();
    setDelayedReply(true);

    const auto windows = workspace()->windows();
    m_pendingWindows.clear();
    m_pendingWindows.reserve(windows.size());
    for (const auto window : windows) {
        if (auto toplevelWindow = qobject_cast<XdgToplevelWindow *>(window)) {
            connect(toplevelWindow, &XdgToplevelWindow::closed, m_closingWindowsGuard.get(), [this, toplevelWindow, dbusMessage] {
                m_pendingWindows.removeOne(toplevelWindow);
                if (m_pendingWindows.empty()) {
#if KWIN_BUILD_NOTIFICATIONS
                    if (m_cancelNotification) {
                        m_cancelNotification->close();
                        m_cancelNotification = nullptr;
                    }
#endif
                    m_closeTimer.stop();
                    m_logoutAnywayTimer.stop();
                    m_closingWindowsGuard.reset();
                    QDBusConnection::sessionBus().send(dbusMessage.createReply(true));
                } else {
                    updateWaylandCancelNotification();
                }
            });
            m_pendingWindows.push_back(toplevelWindow);
            toplevelWindow->closeWindow();
        }
    }

    if (m_pendingWindows.empty()) {
        m_closingWindowsGuard.reset();
        QDBusConnection::sessionBus().send(dbusMessage.createReply(true));
        return true;
    }

    m_closeTimer.start(std::chrono::seconds(10));
    m_closeTimer.setSingleShot(true);
    connect(&m_closeTimer, &QTimer::timeout, m_closingWindowsGuard.get(), [this, dbusMessage] {
#if KWIN_BUILD_NOTIFICATIONS
        m_cancelNotification = new KNotification("cancellogout", KNotification::DefaultEvent | KNotification::Persistent);
        m_cancelNotification->setComponentName(QStringLiteral("kwin-x11"));
        updateWaylandCancelNotification();
        auto cancel = m_cancelNotification->addAction(i18nc("@action:button", "Cancel Logout"));
        auto quit = m_cancelNotification->addAction(i18nc("@action::button", "Log Out Anyway"));
        connect(cancel, &KNotificationAction::activated, m_closingWindowsGuard.get(), [dbusMessage, this] {
            m_closingWindowsGuard.reset();
            QDBusConnection::sessionBus().send(dbusMessage.createReply(false));
        });
        connect(quit, &KNotificationAction::activated, m_closingWindowsGuard.get(), [dbusMessage, this] {
            m_closingWindowsGuard.reset();
            QDBusConnection::sessionBus().send(dbusMessage.createReply(true));
        });
        connect(m_cancelNotification, &KNotification::closed, m_closingWindowsGuard.get(), [dbusMessage, this] {
            m_closingWindowsGuard.reset();
            QDBusConnection::sessionBus().send(dbusMessage.createReply(false));
        });
        m_cancelNotification->sendEvent();
#else
        m_closingWindowsGuard.reset();
        QDBusConnection::sessionBus().send(dbusMessage.createReply(false));
#endif
    });

    m_logoutAnywayTimer.start(std::chrono::minutes(2));
    m_logoutAnywayTimer.setSingleShot(true);
    connect(&m_logoutAnywayTimer, &QTimer::timeout, m_closingWindowsGuard.get(), [this, dbusMessage] {
        qCInfo(KWIN_CORE) << "Not all windows have closed, logging out anyway";
        m_closingWindowsGuard.reset();
        QDBusConnection::sessionBus().send(dbusMessage.createReply(true));
    });
    return true;
}

void SessionManager::updateWaylandCancelNotification()
{
#if KWIN_BUILD_NOTIFICATIONS
    if (!m_cancelNotification) {
        return;
    }

    QStringList apps;
    apps.reserve(m_pendingWindows.size());
    std::transform(m_pendingWindows.cbegin(), m_pendingWindows.cend(), std::back_inserter(apps), [](const XdgToplevelWindow *window) -> QString {
        const auto service = KService::serviceByDesktopName(window->desktopFileName());
        return QStringLiteral("• ") + (service ? service->name() : window->caption());
    });
    apps.removeDuplicates();

    qCDebug(KWIN_CORE) << "Not closed windows" << apps;
    m_cancelNotification->setText(i18n("The following applications did not close:\n%1\nLogging out anyway in 2 minutes.", apps.join('\n')));
#endif
}

void SessionManager::quit()
{
    qApp->quit();
}

} // namespace

#include "moc_sm.cpp"
