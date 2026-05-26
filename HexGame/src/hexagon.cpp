#include "hexagon.h"
#include <QPainter>
#include "types.h"

void Hexagon::paint(QPainter* painter) {
    tColor = to_string(Game::Color::Empty);

    painter->setRenderHint(QPainter::Antialiasing);
    painter->setBrush(QBrush(tColor));
    painter->setPen(QPen(QColor("#000007"), 1));

    qreal cx = width() / 2.0;
    qreal cy = height() / 2.0;
    qreal radius = qMin(cx, cy);

    QPolygonF hexagon;

    for (int i = 0; i < 6; ++i) {
        qreal angle = i * M_PI / 3.0;
        angle += M_PI / 6.0; 

        qreal x = cx + radius * qCos(angle);
        qreal y = cy + radius * qSin(angle);
        hexagon << QPointF(x, y);
    }

    painter->drawPolygon(hexagon);
}

void Hexagon::setColor(const QColor& color) {
    if (color == tColor)
        return;

    tColor = color;
    update();
    emit colorChanged();
}

QColor Hexagon::color() const {
    return tColor;
}

