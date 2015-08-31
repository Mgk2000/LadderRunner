#ifndef BLOCK_H
#define BLOCK_H
#include "movingobject.h"
class Lift;
class Block : public MovingObject
{
public:
    Block(Play* _field, Texture::Kind _kind);
    void moveStep(float delta);
    virtual void moveLeft();
    virtual void moveRight();
    virtual void doStop();
    Block* blockUnder() const;
    Block* blockAbove() const;
    Lift * liftUnder() const;
};

#endif // BLOCK_H
