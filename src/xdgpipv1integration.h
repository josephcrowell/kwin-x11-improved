/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#pragma once

#include "waylandshellintegration.h"

namespace KWaylandServer
{
class XdgPipV1Interface;
}

namespace KWin
{

class XdgPipV1Integration : public WaylandShellIntegration
{
    Q_OBJECT

public:
    explicit XdgPipV1Integration(QObject *parent = nullptr);

private:
    void registerXdgPipV1(KWaylandServer::XdgPipV1Interface *pip);
    void createXdgPipV1Window(KWaylandServer::XdgPipV1Interface *pip);
};

} // namespace KWin
