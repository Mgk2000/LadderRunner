#ifndef BOMBBLOCK_H
#define BOMBBLOCK_H
#include "block.h"

class BombBlock : public Block
{
public:
    BombBlock(Play* _field, Texture::Kind _kind);
    virtual bool bombed() const {return true;}
};

#endif // BOMBBLOCK_H
