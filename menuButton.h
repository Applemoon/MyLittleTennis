#include <QGraphicsPixmapItem>



class MenuButton : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MenuButton( QString pixName, QGraphicsItem * parent = 0 );
    
signals:
    void pressed();
    
private:
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    
    QString pixmapName;
    QString pixmapNameHovered;
}