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

bool MovingObject::doStop()
{
    bool right  = vx >0;
    vx = 0;
    vy =0;
    y = round(y);
    int ix = x;
    if (!right)
        x = ix;
    else
        x = ix+1;
    if (x > field->ncols-1)
        x = field->ncols-1;
    falling = false;
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
    vy = - 2 * v;
    vx = 0;
    falling = true;
}

void MovingObject::checkFall()
{
    int iy = y;
    float dy = y-iy;
    if (dy <0.2 )
    {
        if (field->hasSurface(x, iy))
            doStop();
    }
}
