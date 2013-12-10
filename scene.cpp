#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <math.h>
#include <QSound>
#include <QApplication>

#include "scene.h"



Scene :: Scene( const QRectF& sceneRect, QObject* parent ) :
    QGraphicsScene( sceneRect, parent ), fps( 100 ), speedMultiplier( 1 ),
    winScore( 10 ), playerScore( 0 ), enemyScore( 0 )
{
    setBackgroundBrush( Qt::black );

    titlePixmap = new QGraphicsPixmapItem( QPixmap( QString( ":/images/Resources/Title.png" ) ) );
    newGameBtn = new MenuButton( ":/images/Resources/NewGame.png" );
    exitBtn = new MenuButton( ":/images/Resources/ExitGame.png" );
    player = new Platform( width(), height() );
    enemy = new Enemy(  width(), height() );
    ball = new Ball( 0, 0, width()/63, width()/63 );
    playerPixmaps.push_back( new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) ) );
    playerPixmaps.push_back( new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) ) );
    playerPixmaps.push_back( new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) ) );
    colonPixmap = new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/colon.png" ) ) );
    enemyPixmaps.push_back( new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) ) );
    enemyPixmaps.push_back( new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) ) );
    enemyPixmaps.push_back( new QGraphicsPixmapItem( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) ) );

    startTimer( 1000 / fps, Qt::PreciseTimer );
    initializeTitle();
}



Scene :: ~Scene()
{
    delete titlePixmap;
    delete newGameBtn;
    delete exitBtn;
    delete player;
    delete enemy;
    delete ball;
    playerPixmaps.clear();
    delete colonPixmap;
    enemyPixmaps.clear();
}



void Scene :: clearScene()
{
    foreach ( QGraphicsItem *item, items() )
    {
        removeItem( item );
    }
}



void Scene :: initializeTitle()
{
    clearScene();
    state = Title;

    addItem( titlePixmap );
    titlePixmap->setPos( ( width() - titlePixmap->boundingRect().width() )/2,
                         ( height() - titlePixmap->boundingRect().height() )/2 );

    titlePixmapScale = 1;
    titleAnimationStep = 0;
    borderStep = 0;
}



void Scene :: initializeMenu()
{
    clearScene();
    state = MainMenu;

    QApplication::setOverrideCursor( Qt::ArrowCursor );

    addItem( newGameBtn );
    newGameBtn->setPos( width()/20, height()/2 );
    connect( newGameBtn, SIGNAL( pressed() ), SLOT( initializeGame() ) );

    addItem( exitBtn );
    exitBtn->setPos( width()/20, newGameBtn->pos().y() + newGameBtn->boundingRect().height()*1.3 );
    connect( exitBtn, SIGNAL( pressed() ), SIGNAL( wannaClose() ) );
}



void Scene :: initializeGame()
{
    clearScene();
    state = Game;

    QApplication::setOverrideCursor( Qt::BlankCursor );

    addItem( player );
    player->setPos( width()/40, height()/2 );

    addItem( enemy );
    enemy->setPos( width() - player->x(), height()/2 );

    addItem( ball );
    ball->setLaunched( false );

    foreach ( QGraphicsPixmapItem* item, playerPixmaps )
    {
        addItem( item );
    }
    addItem( colonPixmap );
    foreach ( QGraphicsPixmapItem* item, enemyPixmaps )
    {
        addItem( item );
    }

    updateScore( player );
    updateScore();
    updateScore( enemy );

    newRound();
}



void Scene :: newRound()
{
    ball->setLaunched( false );
    ball->setPos( width()/2, height()/2 );
    ball->setVX( 0 );
    ball->setVY( 0 );
    speedMultiplier = 1;
    QTimer::singleShot( 1000, ball, SLOT( launch() ) );
}



void Scene :: keyPressEvent(QKeyEvent *event)
{
    switch ( state )
    {
    case Title:
    {
        if ( event->key() == Qt::Key_Space || event->key() == Qt::Key_Escape )
        {
            initializeMenu();
        }
        break;
    }
    case MainMenu:
    {
        if ( event->key() == Qt::Key_Escape )
        {
            exit( 0 );
        }
        break;
    }
    case Game:
    {
        if ( event->key() == Qt::Key_Escape )
        {
            initializeMenu();
            return;
        }
        else if ( event->key() == Qt::Key_R )
        {
            newRound();
        }
        break;
    }
    }
}



void Scene :: mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
    switch ( state )
    {
    case Title:
    {
        break;
    }
    case MainMenu:
    {
        newGameBtn->mouseOver( itemAt( event->scenePos(), QTransform() ) == newGameBtn );
        exitBtn->mouseOver( itemAt( event->scenePos(), QTransform() ) == exitBtn );

        break;
    }
    case Game:
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
        break;
    }
    }
}



void Scene :: timerEvent(QTimerEvent *)
{
    switch ( state )
    {
    case Title:
    {
        const ushort borderScale = 27;
        const ushort lastScale = 200;
        if ( titlePixmapScale < borderScale )
        {
            titlePixmapScale = 1.5*sqrt( titleAnimationStep );
            borderStep = titleAnimationStep;
        }
        else if ( titlePixmapScale < lastScale )
        {
            titlePixmapScale = 0.2*(titleAnimationStep - borderStep)*
                    (titleAnimationStep - borderStep) + borderScale;
        }
        else
        {
            initializeMenu();
            return;
        }

        ++titleAnimationStep;
        titlePixmap->setScale( titlePixmapScale );
        titlePixmap->setOffset( ( -titlePixmap->boundingRect().width() )/2,
                                ( -titlePixmap->boundingRect().height() )/2 );
        break;
    }
    case MainMenu:
    {
        break;
    }
    case Game:
    {
        ball->move();
        checkImpact();
        enemy->thinkNMove( ball, player );
        checkBall();
        break;
    }
    default:
    {
        exit( EXIT_FAILURE );
        break;
    }
    }
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
        QSound::play( ":/sound/Resources/LoseRound.wav" );
        ++enemyScore;
        updateScore( enemy );
    }
    else if ( rightEdge )
    {
        QSound::play( ":/sound/Resources/WinRound.wav" );
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
    QSound::play( ":/sound/Resources/Impact.wav" );

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

    speedMultiplier *= 1.02;
    ball->setVX( ball->getVX() * speedMultiplier );
    ball->setVY( ball->getVY() * speedMultiplier );
}



bool Scene :: checkWin()
{
    const bool playerWin = ( playerScore >= winScore );
    const bool enemyWin = ( enemyScore >= winScore );

    if ( playerWin )
    {
        QSound::play( ":/sound/Resources/WinGame.wav" );
    }

    if ( enemyWin )
    {
        QSound::play( ":/sound/Resources/LoseGame.wav" );
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
            playerPixmaps[i]->setPixmap( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) );
        }
        playerPixmaps.last()->setPixmap( QPixmap( QString ( ":/images/Resources/0.png" ) ) );

        // загрузка счета
        ushort tempPlayerScore = playerScore;
        ushort pixmapPosition = playerPixmaps.size() - 1;

        while ( tempPlayerScore > 0 )
        {
            ushort number = tempPlayerScore % 10;
            playerPixmaps[pixmapPosition]->setPixmap( QPixmap( ":/images/Resources/" +
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
            enemyPixmaps[i]->setPixmap( QPixmap( QString ( ":/images/Resources/Empty.png" ) ) );
        }
        enemyPixmaps.first()->setPixmap( QPixmap( QString ( ":/images/Resources/0.png" ) ) );

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
            enemyPixmaps[i]->setPixmap( ":/images/Resources/" +
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
