/*
    SPDX-FileCopyrightText: 2018 Marco Martin <mart@kde.org>
    SPDX-FileCopyrightText: 2023 Harald Sitter <sitter@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL

*/

#include "tabletmodemanager.h"

#include "main.h"

#include <QDBusConnection>

namespace KWin
{

TabletModeManager::TabletModeManager()
{
    KSharedConfig::Ptr kwinSettings = kwinApp()->config();
    m_settingsWatcher = KConfigWatcher::create(kwinSettings);
    connect(m_settingsWatcher.data(), &KConfigWatcher::configChanged, this, &KWin::TabletModeManager::refreshSettings);
    refreshSettings();

    QDBusConnection::sessionBus().registerObject(QStringLiteral("/org/kde/KWin"),
                                                 QStringLiteral("org.kde.KWin.TabletModeManager"),
                                                 this,
                                                 // NOTE: slots must be exported for properties to work correctly
                                                 QDBusConnection::ExportAllProperties | QDBusConnection::ExportAllSignals | QDBusConnection::ExportAllSlots);
}

void KWin::TabletModeManager::refreshSettings()
{
    KSharedConfig::Ptr kwinSettings = kwinApp()->config();
    KConfigGroup cg = kwinSettings->group(QStringLiteral("Input"));
    const QString tabletModeConfig = cg.readPathEntry("TabletMode", QStringLiteral("auto"));
    const bool oldEffectiveTabletMode = effectiveTabletMode();
    if (tabletModeConfig == QLatin1StringView("on")) {
        m_configuredMode = ConfiguredMode::On;
        if (!m_detecting) {
            Q_EMIT tabletModeAvailableChanged(true);
        }
    } else if (tabletModeConfig == QLatin1StringView("off")) {
        m_configuredMode = ConfiguredMode::Off;
    } else {
        m_configuredMode = ConfiguredMode::Auto;
    }
    if (effectiveTabletMode() != oldEffectiveTabletMode) {
        Q_EMIT tabletModeChanged(effectiveTabletMode());
    }
}

bool TabletModeManager::isTabletModeAvailable() const
{
    return m_detecting;
}

bool TabletModeManager::effectiveTabletMode() const
{
    switch (m_configuredMode) {
    case ConfiguredMode::Off:
        return false;
    case ConfiguredMode::On:
        return true;
    case ConfiguredMode::Auto:
    default:
        return false;
    }
}

bool TabletModeManager::isTablet() const
{
    return m_isTabletMode;
}

void TabletModeManager::setIsTablet(bool tablet)
{
    if (m_isTabletMode == tablet) {
        return;
    }

    const bool oldTabletMode = effectiveTabletMode();
    m_isTabletMode = tablet;
    if (effectiveTabletMode() != oldTabletMode) {
        Q_EMIT tabletModeChanged(effectiveTabletMode());
    }
}

void KWin::TabletModeManager::setTabletModeAvailable(bool detecting)
{
    if (m_detecting == detecting) {
        return;
    }

    m_detecting = detecting;
    Q_EMIT tabletModeAvailableChanged(isTabletModeAvailable());
}

KWin::TabletModeManager::ConfiguredMode KWin::TabletModeManager::configuredMode() const
{
    return m_configuredMode;
}

} // namespace KWin

#include "input.h"
#include "moc_tabletmodemanager.cpp"
