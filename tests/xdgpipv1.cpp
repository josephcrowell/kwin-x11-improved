/*
    SPDX-FileCopyrightText: 2015 Martin Gräßlin <mgraesslin@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "KWayland/Client/compositor.h"
#include "KWayland/Client/connection_thread.h"
#include "KWayland/Client/event_queue.h"
#include "KWayland/Client/registry.h"
#include "KWayland/Client/shm_pool.h"
#include "KWayland/Client/xdgshell.h"

#include <QDebug>
#include <QGuiApplication>
#include <QImage>
#include <QPainter>
#include <QThread>

#include "qwayland-wayland.h"
#include "qwayland-xdg-pip-v1.h"
#include "qwayland-xdg-shell.h"

using namespace KWayland::Client;

class XdgPipV1Surface;

class Display : public QObject
{
    Q_OBJECT

public:
    explicit Display(QObject *parent = nullptr);
    ~Display() override;

    void initialize(Registry *registry);

    QThread *m_connectionThread;
    ConnectionThread *m_connectionThreadObject;
    EventQueue *m_eventQueue = nullptr;
    Compositor *m_compositor = nullptr;
    ShmPool *m_shm = nullptr;

    QtWayland::xdg_wm_base *m_xdgWmBase = nullptr;
    QtWayland::xdg_wm_pip_v1 *m_xdgWmPip = nullptr;

    XdgPipV1Surface *m_pipSurface = nullptr;
};

class XdgPipV1Surface : public QObject, public QtWayland::wl_surface, public QtWayland::xdg_surface, public QtWayland::xdg_pip_v1
{
    Q_OBJECT

public:
    XdgPipV1Surface(Display *display)
        : m_display(display)
    {
        wl_surface::init(wl_compositor_create_surface(*display->m_compositor));
        xdg_surface::init(xdg_wm_base_get_xdg_surface(display->m_xdgWmBase->object(), wl_surface::object()));
        xdg_pip_v1::init(xdg_wm_pip_v1_get_xdg_pip(display->m_xdgWmPip->object(), xdg_surface::object()));
    }

    ~XdgPipV1Surface() override
    {
        if (xdg_pip_v1::object()) {
            xdg_pip_v1::destroy();
        }
        if (xdg_surface::object()) {
            xdg_surface::destroy();
        }
        if (wl_surface::object()) {
            wl_surface::destroy();
        }
    }

protected:
    void xdg_surface_configure(uint32_t serial) override
    {
        xdg_surface::ack_configure(serial);
    }

    void xdg_pip_v1_configure_bounds(int32_t width, int32_t height) override
    {
    }

    void xdg_pip_v1_configure(int32_t width, int32_t height) override
    {
        if (width) {
            m_size.setWidth(width);
        } else {
            m_size.setWidth(256);
        }

        if (height) {
            m_size.setHeight(height);
        } else {
            m_size.setHeight(100);
        }

        auto buffer = m_display->m_shm->getBuffer(m_size, m_size.width() * 4).toStrongRef();
        buffer->setUsed(true);

        QImage image(buffer->address(), m_size.width(), m_size.height(), QImage::Format_ARGB32_Premultiplied);
        image.fill(Qt::red);

        wl_surface::attach(*buffer, 0, 0);
        wl_surface::damage_buffer(0, 0, INT32_MAX, INT32_MAX);
        wl_surface::commit();

        buffer->setUsed(false);
    }

    void xdg_pip_v1_dismissed() override
    {
        QCoreApplication::quit();
    }

private:
    Display *m_display;
    QSize m_size;
};

Display::Display(QObject *parent)
    : QObject(parent)
    , m_connectionThread(new QThread(this))
    , m_connectionThreadObject(new ConnectionThread())
{
    connect(m_connectionThreadObject, &ConnectionThread::connected, this, [this] {
        m_eventQueue = new EventQueue(this);
        m_eventQueue->setup(m_connectionThreadObject);

        initialize(new Registry(this));
    }, Qt::QueuedConnection);

    m_connectionThreadObject->moveToThread(m_connectionThread);
    m_connectionThread->start();

    m_connectionThreadObject->initConnection();
}

Display::~Display()
{
    if (m_xdgWmPip) {
        m_xdgWmPip->destroy();
    }
    if (m_xdgWmBase) {
        m_xdgWmBase->destroy();
    }

    m_connectionThread->quit();
    m_connectionThread->wait();
    m_connectionThreadObject->deleteLater();
}

void Display::initialize(Registry *registry)
{
    connect(registry, &KWayland::Client::Registry::interfaceAnnounced, this, [this, registry](const QByteArray &interfaceName, quint32 name, quint32 version) {
        if (interfaceName == wl_compositor_interface.name) {
            m_compositor = new KWayland::Client::Compositor(this);
            m_compositor->setup(static_cast<wl_compositor *>(wl_registry_bind(*registry, name, &wl_compositor_interface, std::min(version, 4u))));
        } else if (interfaceName == wl_shm_interface.name) {
            m_shm = new KWayland::Client::ShmPool(this);
            m_shm->setup(static_cast<wl_shm *>(wl_registry_bind(*registry, name, &wl_shm_interface, std::min(version, 1u))));
        } else if (interfaceName == xdg_wm_base_interface.name) {
            m_xdgWmBase = new QtWayland::xdg_wm_base(*registry, name, std::min(version, 5u));
        } else if (interfaceName == xdg_wm_pip_v1_interface.name) {
            m_xdgWmPip = new QtWayland::xdg_wm_pip_v1(*registry, name, std::min(version, 1u));
        }
    });

    connect(registry, &KWayland::Client::Registry::interfacesAnnounced, this, [this]() {
        if (!m_xdgWmPip) {
            qFatal("xdg_wm_pip_v1 protocol is unavailable");
        }

        m_pipSurface = new XdgPipV1Surface(this);
        m_pipSurface->commit();
    });

    registry->setEventQueue(m_eventQueue);
    registry->create(m_connectionThreadObject);
    registry->setup();
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    Display client;

    return app.exec();
}

#include "xdgpipv1.moc"
