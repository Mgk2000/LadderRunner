#ifndef BLOCK_H
#define BLOCK_H
#include "movingobject.h"
class Lift;
class Block : public MovingObject
{
public:
    Block(Play* _field, Texture::Kind _kind);
    virtual ~Block() {}
    void moveStep(float delta);
    virtual void moveLeft();
    virtual void moveRight();
    virtual void doStop();
    Block* blockUnder() const;
    Block* blockAbove() const;
    Lift * liftUnder() const;
    virtual bool bombed() const {return false;}
    virtual bool lifted() const {return false;}

};

#endif // BLOCK_H
