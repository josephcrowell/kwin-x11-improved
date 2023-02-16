/*
    SPDX-FileCopyrightText: 2023 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#pragma once

#include "kwin_export.h"

#include <QObject>

#include <memory>

struct wl_resource;

namespace KWaylandServer
{

class Display;
class SeatInterface;
class SurfaceInterface;
class XdgPipV1Interface;
class XdgPipV1InterfacePrivate;
class XdgSurfaceInterface;
class XdgWmPipV1InterfacePrivate;

/**
 * The XdgWmPipV1Interface extension provides clients a way to create picture-in-picture
 * surfaces.
 */
class KWIN_EXPORT XdgWmPipV1Interface : public QObject
{
    Q_OBJECT

public:
    explicit XdgWmPipV1Interface(Display *display, QObject *parent = nullptr);
    ~XdgWmPipV1Interface() override;

    Display *display() const;

Q_SIGNALS:
    void pipCreated(XdgPipV1Interface *pip);

private:
    std::unique_ptr<XdgWmPipV1InterfacePrivate> d;
};

/**
 * The XdgPipV1Interface class represents a picture-in-picture surface.
 *
 * XdgPipV1Interface corresponds to the Wayland interface \c xdg_pip_v1.
 */
class KWIN_EXPORT XdgPipV1Interface : public QObject
{
    Q_OBJECT

public:
    enum class ResizeAnchor {
        None = 0,
        Top = 1,
        Bottom = 2,
        Left = 4,
        TopLeft = 5,
        BottomLeft = 6,
        Right = 8,
        TopRight = 9,
        BottomRight = 10,
    };
    Q_ENUM(ResizeAnchor)

    XdgPipV1Interface(XdgWmPipV1Interface *shell, XdgSurfaceInterface *xdgSurface, wl_resource *resource);
    ~XdgPipV1Interface() override;

    /**
     * Returns the XdgSurfaceInterface associated with the XdgPipV1Interface.
     */
    XdgSurfaceInterface *xdgSurface() const;

    /**
     * Returns the SurfaceInterface associated with the XdgPipV1Interface.
     */
    SurfaceInterface *surface() const;

    /**
     * Returns the desktop file name of the pip surface.
     */
    QString applicationId() const;

    /**
     * Sends a configure event to the client. \a size specifies the new window geometry size. A size
     * of zero means the client should decide its own window dimensions.
     */
    quint32 sendConfigure(const QSizeF &size);

    /**
     * Sends a close event to the client. The client may choose to ignore this request.
     */
    void sendDismissed();

    /**
     * Sends an event to the client specifying the maximum bounds for the surface size. Must be
     * called before sendConfigure().
     */
    void sendConfigureBounds(const QSizeF &size);

    /**
     * Returns the XdgPipV1Interface for the specified wayland resource object \a resource.
     */
    static XdgPipV1Interface *get(::wl_resource *resource);

Q_SIGNALS:
    /**
     * This signal is emitted when the xdg-pip-v1 is about to be destroyed.
     */
    void aboutToBeDestroyed();

    /**
     * This signal is emitted when the xdg-pip-v1 has commited the initial state and wants to
     * be configured. After initializing the pip surface, you must send a configure event.
     */
    void initializeRequested();

    /**
     * This signal is emitted when the pip surface has been unmapped and its state has been reset.
     */
    void resetOccurred();

    /**
     * This signal is emitted when the pip wants to be interactively moved. The \a seat and
     * the \a serial indicate the user action in response to which this request has been issued.
     */
    void moveRequested(SeatInterface *seat, quint32 serial);

    /**
     * This signal is emitted when the pip wants to be interactively resized by dragging
     * the specified \a anchor. The \a seat and the \a serial indicate the user action
     * in response to which this request has been issued.
     */
    void resizeRequested(SeatInterface *seat, ResizeAnchor anchor, quint32 serial);

    /**
     * This signal is emitted when the application id changes.
     */
    void applicationIdChanged();

private:
    std::unique_ptr<XdgPipV1InterfacePrivate> d;
};

} // namespace KWaylandServer
