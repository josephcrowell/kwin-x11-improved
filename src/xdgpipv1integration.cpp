/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "xdgpipv1integration.h"
#include "wayland/display.h"
#include "wayland/xdgpip_v1_interface.h"
#include "wayland_server.h"
#include "workspace.h"
#include "xdgpipv1window.h"

using namespace KWaylandServer;

namespace KWin
{

XdgPipV1Integration::XdgPipV1Integration(QObject *parent)
    : WaylandShellIntegration(parent)
{
    XdgWmPipV1Interface *shell = new XdgWmPipV1Interface(waylandServer()->display(), this);
    connect(shell, &XdgWmPipV1Interface::pipCreated,
            this, &XdgPipV1Integration::registerXdgPipV1);
}

void XdgPipV1Integration::registerXdgPipV1(XdgPipV1Interface *pip)
{
    createXdgPipV1Window(pip);
    connect(pip, &XdgPipV1Interface::resetOccurred, this, [this, pip] {
        createXdgPipV1Window(pip);
    });
}

void XdgPipV1Integration::createXdgPipV1Window(XdgPipV1Interface *pip)
{
    if (!workspace()) {
        qCWarning(KWIN_CORE, "An xdg-pip-v1 surface has been created while the compositor "
                             "is still not fully initialized. That is a compositor bug!");
        return;
    }

    Q_EMIT windowCreated(new XdgPipV1Window(pip));
}

} // namespace KWin
