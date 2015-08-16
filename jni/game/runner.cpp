#include "runner.h"
#include "math.h"
#include "math_helper.h"
#include "play.h"
#include "block.h"
#include "logmsg.h"
Runner::Runner(Play* _field) : MovingObject(_field, Texture::RUNNER),
    climbing(false), alive(true), phaseLength(40), lastLeft(false)
{
    v = 5.0;
}

void Runner::climb(int _x, int _y)
{
    climbX = _x;
    climbY = _y;
    double fi = atan2(_x - x, _y-y);
    vx = v* sin(fi);
    vy = v * cos(fi);
    climbing = true;
    lastLeft = vx<0;
}

void Runner::checkClimb()
{
  catchBonus();
    if (dist2(x,y, climbX,climbY) < 0.04)
    {
        climbing = false;
        doStop();
    }
}

bool Runner::busy() const
{
    return MovingObject::busy() || climbing;
}

void Runner::moveStep(float delta)
{
    if (falling)
    {
        if (checkFall(delta));
            catchBonus();
    }
    if (climbing)
    {
        x = x + vx * delta;
        y = y + vy * delta;
        checkClimb();
    }
    if (!busy())
    {
        if (vx < 0)
            tryMoveLeft(delta);
        else if (vx > 0)
            tryMoveRight(delta);
        catchBonus();
        if (!field->hasSurface(x, y))
            fall();
    }
    if (stopping)
        doStop();
}

void Runner::catchBonus()
{
    switch (this->field->cell(x,y)->kind)
    {
    case Texture::GOLDEN_KEY:
        field->nRunnerKeys++;
        field->cell(x,y)->setKind(Texture::EMPTY);
        if (field->nLevelKeys == field->nRunnerKeys)
            field->openDoor();
        break;
    case Texture::BOMB:
        field->nRunnerBombs++;
        field->cell(x,y)->setKind(Texture::EMPTY);
        break;
    case Texture::OPEN_DOOR:
        field->doLevelDone();
        break;
    default:
        break;
    }
}

void Runner::doStop()
{
    MovingObject::doStop();
    climbing = false;
}

void Runner::stop()
{
    if (!busy())
        MovingObject::stop();
    for (std::list<Block*>::iterator bit = field->blocks.begin(); bit != field->blocks.end(); bit++ )
        if (!(*bit)->falling)
            (*bit)->stop();
}

void Runner::die()
{
    this->kind = Texture::DEAD_RUNNER;
    endTime = field->currTime() + 3000;
    alive = false;
}

bool Runner::out() const
{
    return !alive && field->currTime() > endTime;
}

void Runner::moveLeft()
{
    lastLeft = true;
    beginWalkTime = field->currTime();
    MovingObject::moveLeft();
    Block* block = field->blockOfXY(x-1, y);
    if (block)
        block->moveLeft();

}

void Runner::moveRight()
{
    lastLeft = false;
    beginWalkTime = field->currTime();
    MovingObject::moveRight();
    Block* block = field->blockOfXY(x+1, y);
    if (block)
        block->moveRight();
}

void Runner::tryMoveLeft(float delta)
{
    Block* block, *nextblock;
    float nextx = x + vx * delta;
    int ix = nextx;
    float dx = nextx - ix;
//    LOGD("nextx=%f dx=%f", nextx, dx);
    if (ix < 0)
    {
        doStop();
        return;
    }
    if (!field->cell(ix, y)->free())
    {
        doStop();
        return;
    }
    block = field->blockOfXY(nextx,y);
    if (!block)
        goto move;
    block->vx = vx;
    if (!block->canMoveLeft())
    {
        doStop();
        block->stop();
        return;
    }
    else
        block->moveLeft();
    move:
    x = nextx;
    if (x<0)
        x = 0;
}

void Runner::tryMoveRight(float delta)
{
    Block* block, *nextblock;
    float nextx = x + vx * delta;
    int ix = nextx;
    float dx = nextx - ix;
//    LOGD("nextx=%f dx=%f", nextx, dx);
    if (ix == field->ncols-1)
    {
        doStop();
        return;
    }
    if (!field->cell(ix+1, y)->free())
    {
        doStop();
        return;
    }
    block = field->blockOfXY(nextx+1,y);
    if (!block)
        goto move;
    block->vx = vx;
    if (!block->canMoveRight())
    {
        doStop();
        block->stop();
        return;
    }
    else
        block->moveRight();
    move:
    x = nextx;
}

bool Runner::canMoveLeft() const
{
    int xx = x ;
    int yy = y;
    float dx = x - xx;
    if (dx> 0.1)
        return true;
    if (leftFree(xx, yy))
        return true;
    if (field->isBlock(xx-1, yy) && leftFree(xx-1,yy))
        return true;
    return false;

}

bool Runner::canMoveRight() const
{
    int xx = x  ;
    int yy = y ;
    //float dx = x - xx ;
    //if (dx< 0.5)
    //    return true;
    if (rightFree(xx, yy))
        return true;
    if (field->isBlock(xx+1, yy) && rightFree(xx+1,yy))
        return true;
    return false;

}

int Runner::phase() const
{
    if (fabs(vx) < 0.1 )
    {
        if (!lastLeft)
        return 0;
        else
            return 16;
    }
    int nph = ((field->currTime() - beginWalkTime) / phaseLength) % 16;
    if (vx>0)\
        return nph;
    else return nph + 16;
}
