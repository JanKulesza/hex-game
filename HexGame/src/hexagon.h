#include <QQuickPaintedItem>
#include <QPainter>


class Hexagon : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    Hexagon() {}
    void paint(QPainter* painter) override;
    QColor color() const;
    void setColor(const QColor& color);

signals:
    void colorChanged();

private:
    QColor tColor;
};