#include "menuButton.h"



MenuButton :: MenuButton( QString pixName, QGraphicsItem * parent ) :
    QGraphicsPixmapItem ( parent ), pixmapName( pixName ),
    pixmapNameHovered( pixName )
{
    pixmapNameHovered.insert( pixmapNameHovered.lastIndexOf( "." ), "Hovered" );
    setPixmap( QPixmap( pixmapName ) );
}



void MenuButton :: mousePressEvent( QGraphicsSceneMouseEvent * )
{
    emit pressed();
}



void MenuButton :: mouseOver( bool mo )
{
    mo ? setPixmap( QPixmap( pixmapNameHovered ) )
       : setPixmap( QPixmap( pixmapName ) );
}
