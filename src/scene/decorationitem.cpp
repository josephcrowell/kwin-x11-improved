/*
    SPDX-FileCopyrightText: 2014 Martin Gräßlin <mgraesslin@kde.org>
    SPDX-FileCopyrightText: 2021 Vlad Zahorodnii <vlad.zahorodnii@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "scene/decorationitem.h"
#include "compositor.h"
#include "core/output.h"
#include "decorations/decoratedwindow.h"
#include "scene/outlinedborderitem.h"
#include "scene/workspacescene.h"
#include "window.h"

#include <cmath>

#include <KDecoration3/DecoratedWindow>
#include <KDecoration3/Decoration>

namespace KWin
{

DecorationRenderer::DecorationRenderer(Decoration::DecoratedWindowImpl *client)
    : m_client(client)
    , m_imageSizesDirty(true)
{
    connect(client->decoration(), &KDecoration3::Decoration::damaged,
            this, &DecorationRenderer::addDamage);

    connect(client->decoration(), &KDecoration3::Decoration::bordersChanged,
            this, &DecorationRenderer::invalidate);
    connect(client->decoratedWindow(), &KDecoration3::DecoratedWindow::sizeChanged,
            this, &DecorationRenderer::invalidate);

    invalidate();
}

Decoration::DecoratedWindowImpl *DecorationRenderer::client() const
{
    return m_client;
}

void DecorationRenderer::invalidate()
{
    if (m_client) {
        addDamage(m_client->window()->rect().toAlignedRect());
    }
    m_imageSizesDirty = true;
}

QRegion DecorationRenderer::damage() const
{
    return m_damage;
}

void DecorationRenderer::addDamage(const QRegion &region)
{
    m_damage += region;
    Q_EMIT damaged(region);
}

void DecorationRenderer::resetDamage()
{
    m_damage = QRegion();
}

qreal DecorationRenderer::effectiveDevicePixelRatio() const
{
    // QPainter won't let paint with a device pixel ratio less than 1.
    return std::max(qreal(1.0), devicePixelRatio());
}

qreal DecorationRenderer::devicePixelRatio() const
{
    return m_devicePixelRatio;
}

void DecorationRenderer::setDevicePixelRatio(qreal dpr)
{
    if (m_devicePixelRatio != dpr) {
        m_devicePixelRatio = dpr;
        invalidate();
    }
}

void DecorationRenderer::renderToPainter(QPainter *painter, const QRectF &rect)
{
    client()->decoration()->paint(painter, rect);
}

DecorationItem::DecorationItem(KDecoration3::Decoration *decoration, Window *window, Item *parent)
    : Item(parent)
    , m_window(window)
    , m_decoration(decoration)
{
    m_renderer = Compositor::self()->scene()->createDecorationRenderer(window->decoratedWindow());

    connect(window, &Window::targetScaleChanged, this, &DecorationItem::updateScale);

    connect(decoration->window(), &KDecoration3::DecoratedWindow::sizeChanged,
            this, &DecorationItem::handleDecorationGeometryChanged);
    connect(decoration, &KDecoration3::Decoration::bordersChanged,
            this, &DecorationItem::handleDecorationGeometryChanged);
    connect(decoration, &KDecoration3::Decoration::borderOutlineChanged,
            this, &DecorationItem::updateOutline);

    connect(renderer(), &DecorationRenderer::damaged,
            this, qOverload<const QRegion &>(&Item::scheduleRepaint));

    setSize(decoration->size());
    updateScale();
    updateOutline();
}

DecorationItem::~DecorationItem()
{
}

QList<QRectF> DecorationItem::shape() const
{
    QRectF left, top, right, bottom;
    m_window->layoutDecorationRects(left, top, right, bottom);
    return {left, top, right, bottom};
}

QRegion DecorationItem::opaque() const
{
    if (m_window->decorationHasAlpha()) {
        return QRegion();
    }
    QRectF left, top, right, bottom;
    m_window->layoutDecorationRects(left, top, right, bottom);

    // We have to map to integers which has rounding issues
    // it's safer for a region to be considered transparent than opaque
    // so always align inwards
    const QMargins roundingPad = QMargins(1, 1, 1, 1);
    QRegion roundedLeft = left.toAlignedRect().marginsRemoved(roundingPad);
    QRegion roundedTop = top.toAlignedRect().marginsRemoved(roundingPad);
    QRegion roundedRight = right.toAlignedRect().marginsRemoved(roundingPad);
    QRegion roundedBottom = bottom.toAlignedRect().marginsRemoved(roundingPad);

    return roundedLeft | roundedTop | roundedRight | roundedBottom;
}

void DecorationItem::preprocess()
{
    const QRegion damage = m_renderer->damage();
    if (!damage.isEmpty()) {
        m_renderer->render(damage);
        m_renderer->resetDamage();
    }
}

void DecorationItem::updateScale()
{
    const double scale = m_window->targetScale();
    if (m_renderer->devicePixelRatio() != scale) {
        m_renderer->setDevicePixelRatio(scale);
        discardQuads();
    }
}

void DecorationItem::updateOutline()
{
    if (m_decoration->borderOutline().isNull()) {
        m_outlineItem.reset();
    } else {
        const auto outline = BorderOutline::from(m_decoration->borderOutline());
        if (!m_outlineItem) {
            m_outlineItem = std::make_unique<OutlinedBorderItem>(rect(), outline, this);
        } else {
            m_outlineItem->setOutline(outline);
        }
    }
}

void DecorationItem::handleDecorationGeometryChanged()
{
    setSize(m_decoration->size());
    discardQuads();

    if (m_outlineItem) {
        m_outlineItem->setInnerRect(rect());
    }
}

DecorationRenderer *DecorationItem::renderer() const
{
    return m_renderer.get();
}

Window *DecorationItem::window() const
{
    return m_window;
}

WindowQuad buildQuad(const QRectF &partRect, const QPoint &textureOffset,
                     const qreal devicePixelRatio, bool rotated)
{
    const int p = DecorationRenderer::TexturePad;

    const double x0 = partRect.x();
    const double y0 = partRect.y();
    const double x1 = partRect.x() + partRect.width();
    const double y1 = partRect.y() + partRect.height();

    WindowQuad quad;
    if (rotated) {
        const int u0 = textureOffset.y() + p;
        const int v0 = textureOffset.x() + p;
        const int u1 = textureOffset.y() + p + std::round(partRect.width() * devicePixelRatio);
        const int v1 = textureOffset.x() + p + std::round(partRect.height() * devicePixelRatio);

        quad[0] = WindowVertex(x0, y0, v0, u1); // Top-left
        quad[1] = WindowVertex(x1, y0, v0, u0); // Top-right
        quad[2] = WindowVertex(x1, y1, v1, u0); // Bottom-right
        quad[3] = WindowVertex(x0, y1, v1, u1); // Bottom-left
    } else {
        const int u0 = textureOffset.x() + p;
        const int v0 = textureOffset.y() + p;
        const int u1 = textureOffset.x() + p + std::round(partRect.width() * devicePixelRatio);
        const int v1 = textureOffset.y() + p + std::round(partRect.height() * devicePixelRatio);

        quad[0] = WindowVertex(x0, y0, u0, v0); // Top-left
        quad[1] = WindowVertex(x1, y0, u1, v0); // Top-right
        quad[2] = WindowVertex(x1, y1, u1, v1); // Bottom-right
        quad[3] = WindowVertex(x0, y1, u0, v1); // Bottom-left
    }
    return quad;
}

WindowQuadList DecorationItem::buildQuads() const
{
    if (m_window->frameMargins().isNull()) {
        return WindowQuadList();
    }

    QRectF left, top, right, bottom;
    const qreal devicePixelRatio = m_renderer->effectiveDevicePixelRatio();
    const int texturePad = DecorationRenderer::TexturePad;

    m_window->layoutDecorationRects(left, top, right, bottom);

    const int topHeight = std::round(top.height() * devicePixelRatio);
    const int bottomHeight = std::round(bottom.height() * devicePixelRatio);
    const int leftWidth = std::round(left.width() * devicePixelRatio);

    const QPoint topPosition(0, 0);
    const QPoint bottomPosition(0, topPosition.y() + topHeight + (2 * texturePad));
    const QPoint leftPosition(0, bottomPosition.y() + bottomHeight + (2 * texturePad));
    const QPoint rightPosition(0, leftPosition.y() + leftWidth + (2 * texturePad));

    WindowQuadList list;
    if (left.isValid()) {
        list.append(buildQuad(left, leftPosition, devicePixelRatio, true));
    }
    if (top.isValid()) {
        list.append(buildQuad(top, topPosition, devicePixelRatio, false));
    }
    if (right.isValid()) {
        list.append(buildQuad(right, rightPosition, devicePixelRatio, true));
    }
    if (bottom.isValid()) {
        list.append(buildQuad(bottom, bottomPosition, devicePixelRatio, false));
    }
    return list;
}

} // namespace KWin

#include "moc_decorationitem.cpp"
