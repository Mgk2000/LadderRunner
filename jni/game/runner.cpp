#include "runner.h"
#include "math.h"
#include "math_helper.h"
#include "play.h"
Runner::Runner(Play* _field) : MovingObject(_field, Texture::RUNNER),
    climbing(false)
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
}

void Runner::checkClimb()
{
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
        if (checkFall(delta))
            catchBonus();
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
}
