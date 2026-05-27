#include "hexagon.h"
#include <QPainter>
#include "types.h"

void Hexagon::setColor(const QColor& color) {
    if (color == m_color)
        return;

    m_color = color;
    emit colorChanged();
}

QColor Hexagon::color() const {
    return m_color;
}

