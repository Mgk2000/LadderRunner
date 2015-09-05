#include "runner.h"
#include "math.h"
#include "math_helper.h"
#include "play.h"
#include "block.h"
#include "logmsg.h"
#include "ladder.h"
#include "growingcell.h"
#include "lift.h"
Runner::Runner(Play* _field) : MovingObject(_field, Texture::RUNNER),
    climbing(false), alive(true), phaseLength(40), lastLeft(false)
{
    v = 5.0;
    postVX = 0;
    clearNearBlocks();
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
    field->ladder.setCoords(x, y, climbX, climbY, fi);
    if (_x > x)
        postVX = v;
    else if (_x < x)
        postVX = -v;
    else
        postVX = 0;
}

void Runner::checkClimb()
{
  catchBonus();
    if (dist2(x,y, climbX,climbY) < 0.04)
    {
        climbing = false;
        x = climbX;
        y = climbY;
        vx = postVX;
        vy = 0;
        if (vx ==0)
            doStop();
    }
}

bool Runner::busy() const
{
    return MovingObject::busy() || climbing;
}

void Runner::moveStep(float delta)
{
    Cell* cell = field->cell(x,y);
    if (!climbing)
    if (cell->growing() && ((GrowingCell*)cell)->solid())
    {
        this->die();
        return;
    }
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
        if (!field->hasSurface(round(x), y))
            fall();
    }
    if (stopping)
        doStop();
}

void Runner::catchBonus1()
{
    int ix = round(x);
    int iy = round(y);
    switch (this->field->cell(ix,iy)->kind())
    {
    case Texture::GOLDEN_KEY:
        field->nRunnerKeys++;
        field->cell(ix,iy)->setKind(Texture::EMPTY);
        if (field->nLevelKeys == field->nRunnerKeys)
            field->openDoor();
        break;
    case Texture::GRENADE:
        if (fabs(x- int(x)) < 0.2 && fabs(y- int(y)) < 0.2)
        if (ix!= field->grenadePutX || iy!= field->grenadePutY)
        {
            field->nRunnerGrenades++;
            field->cell(ix,iy)->setKind(Texture::EMPTY);
        }
        break;
    case Texture::BOMB:
        field->nRunnerBombs++;
        field->cell(ix,iy)->setKind(Texture::EMPTY);
        break;
    case Texture::OPEN_DOOR:
        if (fabs(x- int(x)) < 0.2 && fabs(y- int(y)) < 0.2)
            field->doLevelDone();
        break;
    case Texture::BULLET_PROOF:
        armored = true;
        field->cell(ix,iy)->setKind(Texture::EMPTY);
        break;
    default:
        field->grenadePutX = -1000;
        break;
    }
}

void Runner::catchBonus()
{
    switch (this->field->cell(x,y)->kind())
    {
    case Texture::GOLDEN_KEY:
        field->nRunnerKeys++;
        field->cell(x,y)->setKind(Texture::EMPTY);
        if (field->nLevelKeys == field->nRunnerKeys)
            field->openDoor();
        break;
    case Texture::GRENADE:
        if (climbing || (fabs(x- int(x)) < 0.2 && fabs(y- int(y)) < 0.2))
        if (round(x)!= field->grenadePutX || round(y)!= field->grenadePutY)
        {
            field->nRunnerGrenades++;
            field->cell(x,y)->setKind(Texture::EMPTY);
        }
        break;
    case Texture::BOMB:
        field->nRunnerBombs++;
        field->cell(x,y)->setKind(Texture::EMPTY);
        break;
    case Texture::OPEN_DOOR:
        if (fabs(x- int(x)) < 0.2 && fabs(y- int(y)) < 0.2)
            field->doLevelDone();
        break;
    case Texture::BULLET_PROOF:
        armored = true;
        field->cell(x,y)->setKind(Texture::EMPTY);
        break;
    default:
        field->grenadePutX = -1000;
        break;
    }
    //catchBonus1();
}

void Runner::doStop()
{
    MovingObject::doStop();
    climbing = false;
    vx = postVX;
}

void Runner::stop()
{
    if (!busy())
        MovingObject::stop();
    for (std::list<Block*>::iterator bit = field->blocks.begin(); bit != field->blocks.end(); bit++ )
        if (!(*bit)->falling)
            (*bit)->stop();
    postVX = 0;
}

void Runner::revive()
{
    this->alive = true;
    field->playing = true;
}

void Runner::die()
{
//    this->setKind (Texture::DEAD_RUNNER);
    endTime = field->currTime() + 6000;
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
    Block* block; //, *nextblock;
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
//    block->vx = vx;
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
    if (!field->cell(ix+1, round(y))->free())
    {
        doStop();
        return;
    }
    block = field->blockOfXY(nextx+1,y);
    if (!block)
        goto move;
//    block->vx = vx;
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

void Runner::fall()
{
    if (field->pressNearLeft || field->pressNearRight)
        postVX = 0;
    else
        postVX = vx;
    MovingObject::fall();
   // vy = -0.3*v;
}

bool Runner::onBlockLift() const
{
    Block* block = field->blockOfXY(x, y-1);
    if (!block)
        return false;
    for (std::list<Lift*>::iterator lit = field->lifts.begin();
         lit != field->lifts.end(); lit++)
    {
        Lift * lift = *lit;
        if (lift->x == round(x) && lift->y1 <= y && lift->y2 >= y )
            return true;
    }
    return false;
}

bool Runner::strongClimbing() const
{
    return climbing && y != climbY;
}

void Runner::calcNearBlocks()
{
    clearNearBlocks();
    std::list<Block*>::iterator bit = field->blocks.begin();
    for (; bit != field->blocks.end(); bit++)
    {
        Block* block = *bit;
        if (fabs(x-block->x) < 0.2 && fabs(y-block->y) < 0.2 )
            inBlock = block;
        else if (fabs(x-block->x) < 0.2 && y - block->y > 0.8 && y - block->y < 1.2)
            bottomBlock = block;
        else if (fabs(x-block->x) < 0.2 && y - block->y > -1.2 && y - block->y < -0.8)
            topBlock = block;
        else if (fabs(y-block->y) < 0.2 && x - block->x > -1.2 && x - block->x < -0.8)
            rightBlock = block;
        else if (fabs(y-block->y) < 0.2 && x - block->x > 0.8 && x - block->x < 1.2)
            leftBlock = block;
    }
}

void Runner::clearNearBlocks()
{
    leftBlock = 0;
    inBlock = 0;
    topBlock = 0;
    rightBlock = 0;
    bottomBlock = 0;

}

bool Runner::checkFall(float delta)
{
    catchBonus();
    MovingObject::checkFall(delta);

}

bool Runner::nearBombBlock() const
{
    if (inBlock && inBlock->bombed())
        return true;
    if (leftBlock && leftBlock->bombed())
        return true;
    if (topBlock && topBlock->bombed())
        return true;
    if (rightBlock && rightBlock->bombed())
        return true;
    if (bottomBlock && bottomBlock->bombed())
        return true;

}
