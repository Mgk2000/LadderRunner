#include "block.h"
#include "play.h"
#include "runner.h"
#include "lift.h"
Block::Block(Play* _field, Texture::Kind _kind)  : MovingObject(_field, _kind)
{
    v = 5.0;
}

void Block::moveStep(float delta)
{
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
        if (!field->hasSurface(x, y) && y>0)
            fall();
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
