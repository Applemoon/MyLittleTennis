#inlude "menuButton.h"



MenuButton( QString pixName, QGraphicsItem * parent ) : QGraphicsItem ( parent ), 
    pixmapName( pixName ), pixmapNameHovered( pixmapName + QString( "Hovered" )
{
    setPixmap( QPixmap( pixmapName ) );
}



void MenuButton :: hoverEnterEvent ( QGraphicsSceneHoverEvent * event )
{
    setPixmap( QPixmap( pixmapNameHovered ) );
}



void MenuButton :: hoverLeaveEvent ( QGraphicsSceneHoverEvent * event )
{
    setPixmap( QPixmap( pixmapName ) );
}



void MenuButton :: mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
    emit pressed();
}