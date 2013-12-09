#ifndef PLATFORM_H
#define PLATFORM_H

#include <QGraphicsObject>



class Platform : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Platform( qreal sceneWidth, qreal sceneHeight,
                       QGraphicsItem * parent = 0 );
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    inline float getHeight() const { return height; }

private:
    float width;
    float height;
};

#endif // PLATFORM_H
