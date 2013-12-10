#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>

#include "enemy.h"
#include "ball.h"
#include "menuButton.h"



class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(const QRectF & sceneRect, QObject * parent = 0);
    ~Scene();

signals:
    void wannaClose();

private:
    enum State { Title, MainMenu, Game };

    void clearScene();
    void initializeTitle();
    void initializeMenu();
    
private slots:
    void initializeGame();
    
private:
    void newRound(); // новая игра
    void keyPressEvent( QKeyEvent *event );
    void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
    void timerEvent( QTimerEvent * ); // событие такта таймера
    void checkImpact(); // проверка столкновения
    void checkBall(); // проверка вылета за границы
    void handleImpact( Platform *platform ); // обработка столкновения
    bool checkWin(); // проверка победы
    void updateScore( Platform *platform = NULL ); // обвновление счета в массиве
    void drawScore( Platform *platform = NULL ); // отрисовка счета

    State state;
    qreal fps;

    // титульник
    QGraphicsPixmapItem *titlePixmap;
    qreal titlePixmapScale;
    uint titleAnimationStep;
    uint borderStep;

    // главное меню
    MenuButton *newGameBtn;
    MenuButton *exitBtn;
    
    // игровые объекты
    Platform *player;
    Enemy *enemy;
    Ball *ball;
    qreal speedMultiplier;

    // табло счета
    ushort winScore;
    ushort playerScore;
    ushort enemyScore;
    QVector< QGraphicsPixmapItem * > playerPixmaps;
    QGraphicsPixmapItem *colonPixmap;
    QVector< QGraphicsPixmapItem * > enemyPixmaps;
};

#endif // SCENE_H
