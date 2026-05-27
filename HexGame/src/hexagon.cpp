#include "hexagon.h"
#include <QPainter>
#include "types.h"

void Hexagon::setColor(Game::Color c) {
    if (c == m_color)
        return;

    m_color = c;
    emit colorChanged();
}

QString Hexagon::getColorStr() {
    return enumToString(m_color);
}

Game::Color Hexagon::getColor() {
    return m_color;
}

