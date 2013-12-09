#ifndef ENEMY_H
#define ENEMY_H

#include "platform.h"
#include "ball.h"



class Enemy : public Platform
{
public:
    Enemy( qreal sceneWidth, qreal sceneHeight, QGraphicsItem * parent = 0 );
    void thinkNMove( const Ball *ball , const Platform *player );

private:
    qreal v;
    qreal accel;
};

#endif // ENEMY_H
