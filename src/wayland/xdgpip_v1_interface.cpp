/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "wayland/xdgpip_v1_interface.h"
#include "wayland/display.h"
#include "wayland/seat_interface.h"
#include "wayland/surfacerole_p.h"
#include "wayland/utils.h"
#include "wayland/xdgshell_interface_p.h"

#include "qwayland-server-xdg-pip-v1.h"

namespace KWaylandServer
{

static const int s_version = 1;

class XdgWmPipV1InterfacePrivate : public QtWaylandServer::xdg_wm_pip_v1
{
public:
    explicit XdgWmPipV1InterfacePrivate(XdgWmPipV1Interface *q, Display *display);

    XdgWmPipV1Interface *q;
    Display *display;

protected:
    void xdg_wm_pip_v1_destroy(Resource *resource) override;
    void xdg_wm_pip_v1_get_xdg_pip(Resource *resource, uint32_t id, struct ::wl_resource *xdg_surface) override;
};

XdgWmPipV1InterfacePrivate::XdgWmPipV1InterfacePrivate(XdgWmPipV1Interface *q, Display *display)
    : QtWaylandServer::xdg_wm_pip_v1(*display, s_version)
    , q(q)
    , display(display)
{
}

void XdgWmPipV1InterfacePrivate::xdg_wm_pip_v1_destroy(Resource *resource)
{
    wl_resource_destroy(resource->handle);
}

void XdgWmPipV1InterfacePrivate::xdg_wm_pip_v1_get_xdg_pip(Resource *resource, uint32_t id, struct ::wl_resource *xdg_surface)
{
    XdgSurfaceInterface *xdgSurface = XdgSurfaceInterface::get(xdg_surface);
    XdgSurfaceInterfacePrivate *xdgSurfacePrivate = XdgSurfaceInterfacePrivate::get(xdgSurface);

    const SurfaceRole *surfaceRole = SurfaceRole::get(xdgSurface->surface());
    if (surfaceRole) {
        wl_resource_post_error(resource->handle, error_already_constructed, "the surface already has a role assigned %s", surfaceRole->name().constData());
        return;
    }

    wl_resource *pipResource = wl_resource_create(resource->client(), &xdg_pip_v1_interface, resource->version(), id);
    xdgSurfacePrivate->pip = new XdgPipV1Interface(q, xdgSurface, pipResource);

    Q_EMIT q->pipCreated(xdgSurfacePrivate->pip);
}

XdgWmPipV1Interface::XdgWmPipV1Interface(Display *display, QObject *parent)
    : QObject(parent)
    , d(std::make_unique<XdgWmPipV1InterfacePrivate>(this, display))
{
}

XdgWmPipV1Interface::~XdgWmPipV1Interface()
{
}

Display *XdgWmPipV1Interface::display() const
{
    return d->display;
}

class XdgPipV1InterfacePrivate : public SurfaceRole, public QtWaylandServer::xdg_pip_v1
{
public:
    XdgPipV1InterfacePrivate(XdgPipV1Interface *q, XdgWmPipV1Interface *shell, XdgSurfaceInterface *xdgSurface);

    void commit() override;
    void reset();

    XdgPipV1Interface *q;
    XdgWmPipV1Interface *shell;
    XdgSurfaceInterface *xdgSurface;
    QString applicationId;

protected:
    void xdg_pip_v1_destroy(Resource *resource) override;
    void xdg_pip_v1_set_app_id(Resource *resource, const QString &app_id) override;
    void xdg_pip_v1_move(Resource *resource, struct ::wl_resource *seat, uint32_t serial) override;
    void xdg_pip_v1_resize(Resource *resource, struct ::wl_resource *seat, uint32_t serial, uint32_t edges) override;
};

XdgPipV1InterfacePrivate::XdgPipV1InterfacePrivate(XdgPipV1Interface *q, XdgWmPipV1Interface *shell, XdgSurfaceInterface *xdgSurface)
    : SurfaceRole(xdgSurface->surface(), QByteArrayLiteral("xdg_pip_v1"))
    , q(q)
    , shell(shell)
    , xdgSurface(xdgSurface)
{
}

void XdgPipV1InterfacePrivate::commit()
{
    auto xdgSurfacePrivate = XdgSurfaceInterfacePrivate::get(xdgSurface);
    if (xdgSurfacePrivate->firstBufferAttached && !xdgSurfacePrivate->surface->buffer()) {
        reset();
        return;
    }

    xdgSurfacePrivate->commit();

    if (!xdgSurfacePrivate->isConfigured) {
        Q_EMIT q->initializeRequested();
    }
}

void XdgPipV1InterfacePrivate::reset()
{
    auto xdgSurfacePrivate = XdgSurfaceInterfacePrivate::get(xdgSurface);
    xdgSurfacePrivate->reset();

    Q_EMIT q->resetOccurred();
}

void XdgPipV1InterfacePrivate::xdg_pip_v1_destroy(Resource *resource)
{
    Q_EMIT q->aboutToBeDestroyed();
    delete q;
}

void XdgPipV1InterfacePrivate::xdg_pip_v1_set_app_id(Resource *resource, const QString &app_id)
{
    if (applicationId != app_id) {
        applicationId = app_id;
        Q_EMIT q->applicationIdChanged();
    }
}

void XdgPipV1InterfacePrivate::xdg_pip_v1_move(Resource *resource, struct ::wl_resource *seat_resource, uint32_t serial)
{
    auto xdgSurfacePrivate = XdgSurfaceInterfacePrivate::get(xdgSurface);

    if (!xdgSurfacePrivate->isConfigured) {
        wl_resource_post_error(resource->handle, QtWaylandServer::xdg_surface::error_not_constructed, "surface has not been configured yet");
        return;
    }

    SeatInterface *seat = SeatInterface::get(seat_resource);
    Q_EMIT q->moveRequested(seat, serial);
}

void XdgPipV1InterfacePrivate::xdg_pip_v1_resize(Resource *resource, struct ::wl_resource *seat_resource, uint32_t serial, uint32_t edges)
{
    auto xdgSurfacePrivate = XdgSurfaceInterfacePrivate::get(xdgSurface);

    if (!xdgSurfacePrivate->isConfigured) {
        wl_resource_post_error(resource->handle, QtWaylandServer::xdg_surface::error_not_constructed, "surface has not been configured yet");
        return;
    }

    SeatInterface *seat = SeatInterface::get(seat_resource);
    Q_EMIT q->resizeRequested(seat, XdgPipV1Interface::ResizeAnchor(edges), serial);
}

XdgPipV1Interface::XdgPipV1Interface(XdgWmPipV1Interface *shell, XdgSurfaceInterface *xdgSurface, wl_resource *resource)
    : d(std::make_unique<XdgPipV1InterfacePrivate>(this, shell, xdgSurface))
{
    d->init(resource);
}

XdgPipV1Interface::~XdgPipV1Interface()
{
}

XdgSurfaceInterface *XdgPipV1Interface::xdgSurface() const
{
    return d->xdgSurface;
}

SurfaceInterface *XdgPipV1Interface::surface() const
{
    return d->xdgSurface->surface();
}

QString XdgPipV1Interface::applicationId() const
{
    return d->applicationId;
}

quint32 XdgPipV1Interface::sendConfigure(const QSizeF &size)
{
    const quint32 serial = d->shell->display()->nextSerial();

    d->send_configure(size.width(), size.height());

    auto xdgSurfacePrivate = XdgSurfaceInterfacePrivate::get(xdgSurface());
    xdgSurfacePrivate->send_configure(serial);
    xdgSurfacePrivate->isConfigured = true;

    return serial;
}

void XdgPipV1Interface::sendDismissed()
{
    d->send_dismissed();
}

void XdgPipV1Interface::sendConfigureBounds(const QSizeF &size)
{
    d->send_configure_bounds(size.width(), size.height());
}

XdgPipV1Interface *XdgPipV1Interface::get(::wl_resource *resource)
{
    if (auto pipPrivate = resource_cast<XdgPipV1InterfacePrivate *>(resource)) {
        return pipPrivate->q;
    }
    return nullptr;
}

} // namespace KWaylandServer
