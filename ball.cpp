#include <QPen>
#include <QBrush>
#include <math.h>

#include "ball.h"



Ball :: Ball( qreal x, qreal y, qreal width, qreal height, QGraphicsItem * parent ) :
    QGraphicsRectItem( x, y, width, height, parent ), vx( 0 ), vy( 0 )
{
    setPen( QPen( Qt::darkGray ) );
    setBrush( QBrush( Qt::white ) );
}



QRectF Ball :: leftSide()
{
    return QRectF( pos().x() - 1, pos().y(), 1, boundingRect().height() );
}



QRectF Ball :: rightSide()
{
    return QRectF( pos().x() + boundingRect().width() + 1, pos().y(),
                   1, boundingRect().height() );
}



QRectF Ball :: topSide()
{
    return QRectF( pos().x(), pos().y() - 1, boundingRect().width(), 1 );
}



QRectF Ball :: bottomSide()
{
    return QRectF( pos().x(), pos().y() + boundingRect().height() + 1,
                   boundingRect().width(), 1 );
}



void Ball :: launch()
{
    if ( !launched )
    {
        qreal tempVX = 20.0;
        qreal tempVY = 7.0;

        if ( qrand() % 2 ) tempVX *= -1;
        if ( qrand() % 2 ) tempVY *= -1;

        setVX( tempVX );
        setVY( tempVY );

        launched = true;
    }
}



void Ball :: move()
{
    setPos( pos().x() + vx, pos().y() + vy );
}
