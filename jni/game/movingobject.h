#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H
#include "cell.h"
class Play;
class MovingObject : public Cell
{
public:
    MovingObject(Play* _field, Texture::Kind _kind);
    virtual bool moveable() const {return true;}

    float x, y, v, vx, vy;
    void setX(float _x) {x=_x;}
    void setY(float _y) {y=_y;}
    void setVX(float _vx) {vx=_vx;}
    void setVY(float _vy) {vy=_vy;}
    virtual void moveLeft();
    virtual void moveRight();
    virtual void stop();
    virtual void  moveStep(float delta);
    Play* field;
    bool stopping;
    virtual void doStop();
    virtual void checkStop();
    void fall();
    bool checkFall(float delta);
    bool falling;
    virtual bool busy() const {return falling || stopping;}
    void tryMoveLeft(float delta);
    void tryMoveRight(float delta);

};

#endif // MOVINGOBJECT_H
