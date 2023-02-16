/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xdgpipv1window.h"
#include "pointer_input.h"
#include "touch_input.h"
#include "virtualdesktops.h"
#include "wayland/seat_interface.h"
#include "wayland/xdgpip_v1_interface.h"
#include "workspace.h"

#if KWIN_BUILD_ACTIVITIES
#include "activities.h"
#endif

using namespace KWaylandServer;

namespace KWin
{

XdgPipV1Window::XdgPipV1Window(XdgPipV1Interface *shellSurface)
    : XdgSurfaceWindow(shellSurface->xdgSurface())
    , m_shellSurface(shellSurface)
{
    m_windowType = NET::Unknown;
    setDesktops({VirtualDesktopManager::self()->currentDesktop()});
#if KWIN_BUILD_ACTIVITIES
    if (auto a = Workspace::self()->activities()) {
        setOnActivities({a->current()});
    }
#endif

    connect(shellSurface, &XdgPipV1Interface::initializeRequested,
            this, &XdgPipV1Window::initialize);
    connect(shellSurface, &XdgPipV1Interface::aboutToBeDestroyed,
            this, &XdgPipV1Window::destroyWindow);
    connect(shellSurface, &XdgPipV1Interface::moveRequested,
            this, &XdgPipV1Window::handleMoveRequested);
    connect(shellSurface, &XdgPipV1Interface::resizeRequested,
            this, &XdgPipV1Window::handleResizeRequested);
    connect(shellSurface, &XdgPipV1Interface::applicationIdChanged,
            this, &XdgPipV1Window::handleApplicationIdChanged);
}

XdgPipV1Window::~XdgPipV1Window()
{
}

void XdgPipV1Window::initialize()
{
    scheduleConfigure();
}

bool XdgPipV1Window::isPictureInPicture() const
{
    return true;
}

bool XdgPipV1Window::isResizable() const
{
    return true;
}

bool XdgPipV1Window::isMovable() const
{
    return true;
}

bool XdgPipV1Window::isMovableAcrossScreens() const
{
    return true;
}

bool XdgPipV1Window::isCloseable() const
{
    return true;
}

void XdgPipV1Window::closeWindow()
{
    m_shellSurface->sendDismissed();
}

bool XdgPipV1Window::wantsInput() const
{
    return false;
}

bool XdgPipV1Window::takeFocus()
{
    return false;
}

bool XdgPipV1Window::acceptsFocus() const
{
    return false;
}

XdgSurfaceConfigure *XdgPipV1Window::sendRoleConfigure() const
{
    const QRectF geometry = moveResizeGeometry();
    if (geometry.isEmpty()) {
        const QRectF workArea = workspace()->clientArea(PlacementArea, this, moveResizeOutput());
        m_shellSurface->sendConfigureBounds(workArea.size() * 0.25);
    }

    XdgSurfaceConfigure *configureEvent = new XdgSurfaceConfigure();
    configureEvent->bounds = moveResizeGeometry();

    configureEvent->serial = m_shellSurface->sendConfigure(geometry.size());
    return configureEvent;
}

void XdgPipV1Window::handleApplicationIdChanged()
{
    setResourceClass(resourceName(), m_shellSurface->applicationId());
    setDesktopFileName(m_shellSurface->applicationId());
}

void XdgPipV1Window::handleMoveRequested(SeatInterface *seat, quint32 serial)
{
    if (!seat->hasImplicitPointerGrab(serial) && !seat->hasImplicitTouchGrab(serial)) {
        return;
    }
    QPointF cursorPos;
    if (seat->hasImplicitPointerGrab(serial)) {
        cursorPos = input()->pointer()->pos();
    } else {
        cursorPos = input()->touch()->position();
    }
    performMouseCommand(Options::MouseMove, cursorPos);
}

void XdgPipV1Window::handleResizeRequested(SeatInterface *seat, XdgPipV1Interface::ResizeAnchor anchor, quint32 serial)
{
    if (!seat->hasImplicitPointerGrab(serial) && !seat->hasImplicitTouchGrab(serial)) {
        return;
    }
    if (isInteractiveMoveResize()) {
        finishInteractiveMoveResize(false);
    }
    setInteractiveMoveResizePointerButtonDown(true);
    QPointF cursorPos;
    if (seat->hasImplicitPointerGrab(serial)) {
        cursorPos = input()->pointer()->pos();
    } else {
        cursorPos = input()->touch()->position();
    }
    setInteractiveMoveOffset(cursorPos - pos()); // map from global
    setInvertedInteractiveMoveOffset(rect().bottomRight() - interactiveMoveOffset());
    setUnrestrictedInteractiveMoveResize(false);
    Gravity gravity;
    switch (anchor) {
    case XdgPipV1Interface::ResizeAnchor::TopLeft:
        gravity = Gravity::TopLeft;
        break;
    case XdgPipV1Interface::ResizeAnchor::Top:
        gravity = Gravity::Top;
        break;
    case XdgPipV1Interface::ResizeAnchor::TopRight:
        gravity = Gravity::TopRight;
        break;
    case XdgPipV1Interface::ResizeAnchor::Right:
        gravity = Gravity::Right;
        break;
    case XdgPipV1Interface::ResizeAnchor::BottomRight:
        gravity = Gravity::BottomRight;
        break;
    case XdgPipV1Interface::ResizeAnchor::Bottom:
        gravity = Gravity::Bottom;
        break;
    case XdgPipV1Interface::ResizeAnchor::BottomLeft:
        gravity = Gravity::BottomLeft;
        break;
    case XdgPipV1Interface::ResizeAnchor::Left:
        gravity = Gravity::Left;
        break;
    default:
        gravity = Gravity::None;
        break;
    }
    setInteractiveMoveResizeGravity(gravity);
    if (!startInteractiveMoveResize()) {
        setInteractiveMoveResizePointerButtonDown(false);
    }
    updateCursor();
}

} // namespace KWin
