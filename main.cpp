/*-------------------------------------------------------
My Little Tennis
Ver. 0.2.1
Applemoon. No rights reserved.
-------------------------------------------------------*/

#include <QApplication>
#include <QGraphicsView>
#include <QDesktopWidget>
#include <QTimer>
#include <QTime>

#include "scene.h"



int main( int argc, char** argv )
{
    QApplication app( argc, argv );

    qsrand( QTime( 0, 0, 0 ).secsTo( QTime::currentTime() ) );

    QRect screenRect( 0, 0, QApplication::desktop()->screenGeometry().width(),
                                QApplication::desktop()->screenGeometry().height() );
    Scene scene( screenRect );

    QGraphicsView view( &scene );
    view.setMouseTracking( true );
    view.setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.setViewportUpdateMode( QGraphicsView::FullViewportUpdate );
    QObject::connect( &scene, SIGNAL( wannaClose() ), &view, SLOT( close() ) );

    view.showFullScreen();

    const int result = app.exec();
    return result;
}
