#ifndef MOVINGOBJECT_H
#define MOVINGOBJECT_H
#include "cell.h"
class Field;
class MovingObject : public Cell
{
public:
    MovingObject(Field* _field, Texture::Kind _kind);
    virtual bool moveable() const {return true;}

    float x, y, v, vx, vy;
    void setX(float _x) {x=_x;}
    void setY(float _y) {y=_y;}
    void setVX(float _vx) {vx=_vx;}
    void setVY(float _vy) {vy=_vy;}
    virtual void moveLeft();
    virtual void moveRight();
    virtual void stop();
    void moveStep(float delta);
    Field* field;
};

#endif // MOVINGOBJECT_H
