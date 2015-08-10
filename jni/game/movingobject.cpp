#include "movingobject.h"
#include "logmsg.h"
#include "play.h"
#include "math.h"

MovingObject::MovingObject(Play* _field, Texture::Kind _kind) : Cell (_kind), field(_field), falling(false)
{
    vy = 0;
}

void MovingObject::moveLeft()
{
    vx = -v;
    stopping = false;
}

void MovingObject::moveRight()
{
    vx = v;
    stopping = false;
}

void MovingObject::stop()
{
    stopping = true;
    return;
    bool right  = vx >0;
    vx = 0;
    vy =0;

    int ix = x;
    if (!right)
        x = ix;
    else
        x = ix+1;
    if (x > field->ncols-1)
        x = field->ncols-1;
}

void MovingObject::moveStep(float delta)
{
    x = x + vx * delta;
    y = y + vy * delta;
    if (stopping)
        checkStop();
//    LOGD("x=%f", x);
    if (x<0)
    {
        x=0;
        stop();
    }
    else if (x>field->ncols-1)
    {
        x = field->ncols-1;
        stop();
    };
    if (y<0)
    {
        y=0;
        stop();
    }
    else if (y>field->nrows-1)
    {
        x = field->nrows-1;
        stop();
    }
}

void MovingObject::doStop()
{
    vx = 0;
    vy =0;
    y = round(y);
    x = round(x);
    falling = false;
    stopping = false;
}

void MovingObject::checkStop()
{
    int ix = x;
    float dx = x-ix;
    //LOGD("stopping x=%f ix=%f dx=%f", )
    if (dx>0.9 || dx < 0.1)
    {
        stopping = false;
        doStop();
    }
}

void MovingObject::fall()
{
    if (vx <=0)
        x = (int) x;
    vy = - 1.3 * v;
    vx = 0;
    falling = true;
}

bool MovingObject::checkFall(float delta)
{
    float nexty = y + vy* delta;
    int iy = y;
    int in = nexty;
    for (int i = iy; i>=in; i--)
        if (field->hasSurface(x, i))
        {
            y = i;
            doStop();
            return true;
        }
    y = nexty;
    return false;
}

void MovingObject::tryMoveLeft(float delta)
{
    float nextx = x + vx * delta;
    int ix = x;
    int iy = y;
    int nix = nextx;
    for (int i = ix-1; i>= nix; i--)
        if (!field->canMoveTo(i-1, iy))
        {
            x = i;
            doStop();
            return;
        }
    x = nextx;
}

void MovingObject::tryMoveRight(float delta)
{
    float nextx = x + vx * delta;
    int ix = x;
    int iy = y;
    int nix = nextx;
    //LOGD("x=%f nix=%d", x, nix)
    for (int i = ix+1; i<= nix; i++)
        if (!field->canMoveTo(i+1, iy))
        {
            x = i;
            doStop();
            return;
        }
    x = nextx;
}
