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
    float saveX, saveY;
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
    virtual void fall();
    bool checkFall(float delta);
    bool falling;
    virtual bool busy() const {return falling || stopping;}
    virtual void tryMoveLeft(float delta);
    virtual void tryMoveRight(float delta);
    virtual bool canMoveLeft() const;
    virtual bool canMoveRight() const;
    bool leftFree(int x, int y) const;
    bool rightFree(int x, int y) const;


};

#endif // MOVINGOBJECT_H
