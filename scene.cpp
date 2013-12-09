#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <math.h>
#include <QSound>

#include "scene.h"



Scene :: Scene( const QRectF& sceneRect, QObject* parent ) :
    QGraphicsScene( sceneRect, parent ), ballLaunched( false ), fps( 100 ),
    speedMultiplier( 1 ), winScore( 20 ), playerScore( 0 ), enemyScore( 0 )
{
    setBackgroundBrush( Qt::black );

    player = new Platform( width(), height() );
    addItem( player );
    player->setPos( width()/40, player->getHeight()/2 );

    enemy = new Enemy(  width(), height() );
    addItem( enemy );
    enemy->setPos( width() - player->x(), height()/2 );

    ball = new Ball( 0, 0, width()/63, width()/63 );
    addItem( ball );

    playerPixmaps.push_back( addPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) ) );
    playerPixmaps.push_back( addPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) ) );
    playerPixmaps.push_back( addPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) ) );
    colonPixmap = addPixmap( QPixmap( QString ( ":/images/colon.png" ) ) );
    enemyPixmaps.push_back( addPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) ) );
    enemyPixmaps.push_back( addPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) ) );
    enemyPixmaps.push_back( addPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) ) );

    updateScore( player );
    updateScore();
    updateScore( enemy );

    startTimer( 1000 / fps, Qt::PreciseTimer );

    newRound();
}



void Scene :: newRound()
{
    ballLaunched = false;
    ball->setPos( width()/2, height()/2 );
    ball->setVX( 0 );
    ball->setVY( 0 );
    speedMultiplier = 1;
    QTimer::singleShot( 1000, this, SLOT( launchBall() ) );
}



void Scene :: keyPressEvent(QKeyEvent *event)
{
    switch ( event->key() )
    {
    case Qt::Key_Escape :
    {
        exit( 0 );
    }
    case Qt::Key_R :
    {
        newRound();
        break;
    }
    }
}



void Scene :: mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    const float dy = event->scenePos().y() - event->lastScenePos().y();
    const bool overBottomBound =
            ( player->y() + player->getHeight()/2 + dy < height() );
    const bool underUpperBound =
            ( player->y() - player->getHeight()/2 + dy > 0 );

    if ( underUpperBound && overBottomBound )
    {
        player->setPos( player->x(), player->y() + dy );
    }
    else if ( !underUpperBound )
    {
        player->setPos( player->x(), player->getHeight()/2 );
    }
    else
    {
        player->setPos( player->x(), height() - player->getHeight()/2 );
    }
}



void Scene :: timerEvent(QTimerEvent *)
{
    ball->setPos( ball->pos().x() + ball->getVX(),
                  ball->pos().y() + ball->getVY() );

    checkImpact();

    enemy->thinkNMove( ball, player );
    checkBall();
}



void Scene :: checkImpact()
{
    if ( ball->pos().x() > player->pos().x() - player->boundingRect().width()/2 &&
         ball->pos().x() < enemy->pos().x() + enemy->boundingRect().width()/2 )
    {

        QList<QGraphicsItem *> items = this->items( ball->leftSide() );
        foreach( QGraphicsItem* item, items )
        {
            if ( item == player  )
            {
                handleImpact( (Platform*)item );
            }
        }

        items = this->items( ball->rightSide() );
        foreach( QGraphicsItem* item, items )
        {
            if ( item == enemy )
            {
                handleImpact( (Platform*)item );
            }
        }
    }

    const bool touchedBottom =
            ( ball->pos().y() + ball->boundingRect().height() >= height() );

    const bool touchedTop =
            ( ball->pos().y() <= 0 );

    if ( touchedBottom || touchedTop )
    {
        ball->invertVY();
    }
}



void Scene :: checkBall()
{
    const bool leftEdge = ( ball->pos().x() < 0 );
    const bool rightEdge =
            ( ball->pos().x() + ball->boundingRect().width() > width() );

    if ( leftEdge )
    {
        QSound::play( ":/sound/LoseRound.wav" );
        ++enemyScore;
        updateScore( enemy );
    }
    else if ( rightEdge )
    {
        QSound::play( ":/sound/WinRound.wav" );
        ++playerScore;
        updateScore( player);
    }

    if ( leftEdge || rightEdge )
    {
        if ( checkWin() )
        {
            playerScore = 0;
            updateScore( player );
            enemyScore = 0;
            updateScore( enemy );
        }

        newRound();
    }
}



void Scene :: handleImpact( Platform *platform )
{
    QSound::play( ":/sound/Impact.wav" );

    qreal absV = sqrt( ball->getVX()*ball->getVX() +
                       ball->getVY()*ball->getVY() );

    qreal ballY = ball->pos().y();
    const qreal platformHeight = platform->boundingRect().height();
    qreal platformY = platform->pos().y();

    qreal percent = 2 * ( ballY - platformY ) / platformHeight;
    const qreal maxAngle = M_PI / 3;
    qreal angleAfter = percent * maxAngle;
    qreal vxAfter = absV * cos( angleAfter );
    qreal vyAfter = absV * sin( angleAfter );

    if ( platform == enemy )
    {
        vxAfter = -abs( vxAfter );
    }

    ball->setVX( vxAfter );
    ball->setVY( vyAfter );

    speedMultiplier *= 1.05;
    ball->setVX( ball->getVX() * speedMultiplier );
    ball->setVY( ball->getVY() * speedMultiplier );
}



bool Scene :: checkWin()
{
    const bool playerWin = ( playerScore >= winScore );
    const bool enemyWin = ( enemyScore >= winScore );

    if ( playerWin )
    {
        QSound::play( ":/sound/WinGame.wav" );
    }

    if ( enemyWin )
    {
        QSound::play( ":/sound/LoseGame.wav" );
    }

    if ( playerWin || enemyWin )
    {
        return true;
    }

    return false;
}



void Scene :: updateScore( Platform *platform )
{
    if ( platform == player )
    {
        // очистка счета
        for ( int i = 0; i < playerPixmaps.size() - 1; ++i )
        {
            playerPixmaps[i]->setPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) );
        }
        playerPixmaps.last()->setPixmap( QPixmap( QString ( ":/images/0.png" ) ) );

        // загрузка счета
        ushort tempPlayerScore = playerScore;
        ushort pixmapPosition = playerPixmaps.size() - 1;

        while ( tempPlayerScore > 0 )
        {
            ushort number = tempPlayerScore % 10;
            playerPixmaps[pixmapPosition]->setPixmap( QPixmap( ":/images/" +
                                                               QString::number( number ) +
                                                               ".png" ) );
            tempPlayerScore /= 10;
            --pixmapPosition;
        }

        drawScore( player );
    }
    else if ( platform == enemy )
    {
        // очистка счета
        for ( int i = 1; i < enemyPixmaps.size(); ++i )
        {
            enemyPixmaps[i]->setPixmap( QPixmap( QString ( ":/images/Empty.png" ) ) );
        }
        enemyPixmaps.first()->setPixmap( QPixmap( QString ( ":/images/0.png" ) ) );

        // загрузка счета
        ushort tempEnemyScore = enemyScore;
        QVector<int> digits;

        while ( tempEnemyScore > 0 )
        {
            ushort number = tempEnemyScore % 10;
            digits.push_front( number );
            tempEnemyScore /= 10;
        }

        for ( int i = 0; i < digits.size(); ++i )
        {
            enemyPixmaps[i]->setPixmap( ":/images/" +
                                        QString::number( digits.at( i ) ) +
                                        ".png" );
        }

        drawScore( enemy );
    }
    else
    {
        drawScore();
    }
}



void Scene :: drawScore( Platform *platform )
{
    qreal scorePixmapWidth = width() / 20;

    if ( platform == player )
    {
        // счет игрока
        for ( short i = 0; i < playerPixmaps.size(); ++i )
        {
            playerPixmaps[i]->setScale( scorePixmapWidth /
                                        playerPixmaps[i]->boundingRect().width() );
            playerPixmaps[i]->setPos( width()/2 - scorePixmapWidth*( 0.5 +
                                      playerPixmaps.size() - i ), height()/20 );
        }
    }
    else if ( platform == enemy )
    {
        // счет врага
        for ( short i = 0; i < enemyPixmaps.size(); ++i )
        {
            enemyPixmaps[i]->setScale( scorePixmapWidth /
                                       enemyPixmaps[i]->boundingRect().width() );
            enemyPixmaps[i]->setPos( width()/2 + scorePixmapWidth*( 0.5 + i ),
                                     height()/20 );
        }
    }
    else
    {
        // двоеточие
        colonPixmap->setScale( scorePixmapWidth /
                               colonPixmap->boundingRect().width() );
        colonPixmap->setPos( width()/2 - scorePixmapWidth/2, height()/20 );
    }
}



void Scene :: launchBall()
{
    if ( !ballLaunched )
    {
        int vx = 15.0;
        int vy = 10.0;

        const bool left = qrand() % 2;
        if ( left ) vx *= -1;

        const bool up = qrand() % 2;
        if ( up ) vy *= -1;

        ball->setVX( vx );
        ball->setVY( vy );

        ballLaunched = true;
    }
}
