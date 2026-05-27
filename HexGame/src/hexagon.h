#pragma once
#include <QObject>
#include <QColor>

class Hexagon : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit Hexagon(QObject* parent = nullptr): QObject(parent) {};
    QColor color() const;
    void setColor(const QColor& color);


signals:
    void colorChanged();

private:
    QColor m_color;
};