#include "block.h"
#include "play.h"
#include "runner.h"
#include "lift.h"
#include "math.h"
Block::Block(Play* _field, Texture::Kind _kind)  : MovingObject(_field, _kind)
{
    v = 5.0;
}

void Block::moveStep(float delta)
{
    if (x>999)
        return;
    if (falling)
        if (checkFall(delta))
            field->runner->catchBonus();
    if (!busy())
    {
        if (vx < 0)
            tryMoveLeft(delta);
        else if (vx > 0)
            tryMoveRight(delta);
//        catchBonus();
        if (!field->hasSurface(round(x), y) && y>0)
        {
            //Block* block = field->blockOfXY(x, y-1);
            Block* block = blockUnder();
            if (!block)
                if (fabs(field->runner->x - x) >=1 ||
                        round(field->runner->y) != round(y-1) )
            {
                Lift * lift = liftUnder();
                if (!lift)
                {
                    block = blockUnder();
                    lift = liftUnder();
                    bool b = field->hasSurface(x, y);
                    fall();
                }

            }
        }
    }
    if (stopping)
        doStop();
}

void Block::moveLeft()
{
    if (abs(vx) < 0.1)
        field->undo.save();
    MovingObject::moveLeft();
    Lift* lift = field->liftOfXY(x, y);
    if (lift)
        lift->block = 0;

}

void Block::moveRight()
{
    if (abs(vx) < 0.1)
        field->undo.save();
    MovingObject::moveRight();
    Lift* lift = field->liftOfXY(x, y);
    if (lift)
        lift->block = 0;
}

void Block::doStop()
{
    MovingObject::doStop();
    Lift* lift = field->liftOfXY(x, y);
    if (lift)
        lift->block = this;

}

Block *Block::blockUnder() const
{
    for (std::list<Block*>::const_iterator bit = field->blocks.begin();
         bit != field->blocks.end(); bit++)
    {
        if (*bit == this )
            continue;
        Block* block = *bit;
        if (fabs(x-block->x) >0.1)
            continue;
        if (y- block->y>1)
            continue;
        if (y- block->y< -0.5)
            continue;
        return block;
    }
    return 0;
}

Block *Block::blockAbove() const
{
    for (std::list<Block*>::const_iterator bit = field->blocks.begin();
         bit != field->blocks.end(); bit++)
    {
        if (*bit == this )
            continue;
        Block* block = *bit;
        if (abs(x-block->x) >0.1)
            continue;
        if (block->y - y >1.3)
            continue;
        if (block->y - y < -0.5)
            continue;
        return block;
    }
    return 0;

}

Lift *Block::liftUnder() const
{
    for (std::list<Lift*>::const_iterator lit = field->lifts.begin();
         lit != field->lifts.end(); lit++)
        if ((*lit)->block == this)
            return *lit;
    return 0;

}
