#ifndef LIFTBLOCK_H
#define LIFTBLOCK_H
#include "block.h"

class LiftBlock : public Block
{
public:
    LiftBlock(Play* _field, Texture::Kind _kind);
    virtual bool lifted() const {return true;}

};

#endif // LIFTBLOCK_H
