#include <QGraphicsScene>

#include "enemy.h"



Enemy :: Enemy( qreal sceneWidth, qreal sceneHeight, QGraphicsItem *parent ) :
    Platform( sceneWidth, sceneHeight, parent ), v( 0 ), accel( 5.0 )
{
}



void Enemy :: thinkNMove( const Ball *ball , const Platform *player )
{
    // скорость вниз - положительная, вверх - отрицательная

    const qreal maxV = 15.0;
    //const qreal maxAccel = 2.0;

    // движемся за мячом
    if ( ball->pos().y() > y() )
    {
        v += accel;
    }
    else if ( ball->pos().y() < y() )
    {
        v -= accel;
    }

    // ограничение скорости по максимуму
    if ( v > maxV )
    {
        v = maxV;
    }
    else if ( v < -maxV )
    {
        v = -maxV;
    }

    // ограничение по границам экрана
    if ( pos().y() - boundingRect().height()/2 <= 0 )
    {
        if ( v < 0 )
        {
            v = 0;
        }
    }
    if ( pos().y() + boundingRect().height()/2 >= scene()->height() )
    {
        if ( v > 0 )
        {
            v = 0;
        }
    }

    // не дергаемся, если мяч напротив
    if ( abs( y() - ball->pos().y() ) < boundingRect().height()/2 )
    {
        if ( v > 0 )
            v -= accel;
        else
            v += accel;

        if ( abs( v ) < accel )
        {
            v = 0;
        }
    }

    setPos( x(), y() + v );

    Q_UNUSED( player );
}
