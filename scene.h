#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

#include "enemy.h"
#include "ball.h"



class Scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(const QRectF & sceneRect, QObject * parent = 0);

private:
    enum Dir { Top, Bottom, Left, Right };
    //enum State { Title, MainMenu, Game };

    void newRound(); // новая игра
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void timerEvent(QTimerEvent *); // событие такта таймера
    void checkImpact(); // проверка столкновения
    void checkBall(); // проверка вылета за границы
    void handleImpact( Platform *platform ); // обработка столкновения
    bool checkWin(); // проверка победы
    void updateScore( Platform *platform = NULL ); // обвновление счета в массиве
    void drawScore( Platform *platform = NULL ); // отрисовка счета

    //State state;

    Platform *player;
    Enemy *enemy;
    Ball *ball;

    bool ballLaunched;
    qreal fps;
    qreal speedMultiplier;

    ushort winScore;
    ushort playerScore;
    ushort enemyScore;
    QVector< QGraphicsPixmapItem * > playerPixmaps;
    QGraphicsPixmapItem *colonPixmap;
    QVector< QGraphicsPixmapItem * > enemyPixmaps;

private slots:
    void launchBall();
};

#endif // SCENE_H
