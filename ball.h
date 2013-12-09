#ifndef BALL_H
#define BALL_H

#include <QGraphicsRectItem>
#include <QRectF>



class Ball : public QGraphicsRectItem
{
public:
    explicit Ball( qreal x, qreal y, qreal width, qreal height,
                   QGraphicsItem * parent = 0 );
    inline void setVX( qreal newVX ) { vx = newVX; }
    inline void setVY( qreal newVY ) { vy = newVY; }
    inline void invertVX() { vx *= -1; }
    inline void invertVY() { vy *= -1; }
    inline qreal getVX() const { return vx; }
    inline qreal getVY() const { return vy; }

    QRectF leftSide();
    QRectF rightSide();
    QRectF topSide();
    QRectF bottomSide();

private:
    qreal vx;
    qreal vy;
};

#endif // BALL_H
