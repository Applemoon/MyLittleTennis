#include <QGraphicsPixmapItem>



class MenuButton : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    MenuButton( QString pixName, QGraphicsItem * parent = 0 );
    void mouseOver( bool mo );
    
signals:
    void pressed();
    
private:
    void mousePressEvent( QGraphicsSceneMouseEvent * event );
    
    QString pixmapName;
    QString pixmapNameHovered;
};
