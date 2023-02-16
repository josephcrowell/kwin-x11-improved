/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "wayland/xdgpip_v1_interface.h"
#include "xdgshellwindow.h"

namespace KWin
{

class XdgPipV1Window final : public XdgSurfaceWindow
{
    Q_OBJECT

public:
    explicit XdgPipV1Window(KWaylandServer::XdgPipV1Interface *shellSurface);
    ~XdgPipV1Window() override;

    bool isPictureInPicture() const override;
    bool isResizable() const override;
    bool isMovable() const override;
    bool isMovableAcrossScreens() const override;
    bool isCloseable() const override;
    void closeWindow() override;
    bool wantsInput() const override;
    bool takeFocus() override;

protected:
    bool acceptsFocus() const override;
    XdgSurfaceConfigure *sendRoleConfigure() const override;

private:
    void initialize();
    void handleApplicationIdChanged();
    void handleMoveRequested(KWaylandServer::SeatInterface *seat, quint32 serial);
    void handleResizeRequested(KWaylandServer::SeatInterface *seat, KWaylandServer::XdgPipV1Interface::ResizeAnchor anchor, quint32 serial);

    KWaylandServer::XdgPipV1Interface *m_shellSurface;
};

} // namespace KWin
