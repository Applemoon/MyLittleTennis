#include <QPainter>
#include <QStyleOptionGraphicsItem>

#include "platform.h"



Platform :: Platform( qreal sceneWidth, qreal sceneHeight, QGraphicsItem * parent ) :
    QGraphicsObject( parent ), width( sceneWidth/80 ), height( sceneHeight/6 )
{
}



QRectF Platform :: boundingRect() const
{
    return QRectF( -width/2, -height/2, width, height );
}



void Platform :: paint( QPainter *painter, const QStyleOptionGraphicsItem *option,
                       QWidget * )
{
    painter->setClipRect( option->exposedRect );
    painter->setBrush( Qt::white );
    painter->setPen( QPen( Qt::darkGray ) );
    painter->drawRect( boundingRect() );
}
