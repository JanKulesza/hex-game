#pragma once
#include <QObject>
#include <QString>
#include "types.h"

class Hexagon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString color READ getColorStr NOTIFY colorChanged)

public:
    Hexagon(QObject* parent = nullptr): QObject(parent), m_color(Game::Color::Empty) {};
    QString getColorStr();
    Game::Color getColor();
    void setColor(Game::Color color);

signals:
    void colorChanged();

private:
    Game::Color m_color;
};