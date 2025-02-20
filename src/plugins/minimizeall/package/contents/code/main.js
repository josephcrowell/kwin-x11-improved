/*
    KWin - the KDE window manager
    This file is part of the KDE project.

    SPDX-FileCopyrightText: 2014 Thomas Lübking <thomas.luebking@gmail.com>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

console.warn("-------------------------------------- MinimizeAll")

callDBus("org.freedesktop.systemd1", "/", "org.freedesktop.DBus.Peer", "Ping")
callDBus("org.freedesktop.systemd1", "/", "org.freedesktop.DBus.Peer", "Ping", (result) => {
    console.log("callback", result)
})
callDBus("org.freedesktop.systemd1", "/", "org.freedesktop.DBus.Peer", "Invalid")

callDBus("org.freedesktop.systemd1", "/org/freedesktop/systemd1", "org.freedesktop.DBus.Properties", "GetAll", "org.freedesktop.systemd1.Manager", (result) => {
    console.log("Props callback", JSON.stringify(result))
})
