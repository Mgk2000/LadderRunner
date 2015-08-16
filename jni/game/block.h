#ifndef BLOCK_H
#define BLOCK_H
#include "movingobject.h"

class Block : public MovingObject
{
public:
    Block(Play* _field, Texture::Kind _kind);
    void moveStep(float delta);
};

#endif // BLOCK_H
