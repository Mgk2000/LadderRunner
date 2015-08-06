#include "movingobject.h"
#include "logmsg.h"
#include "field.h"

MovingObject::MovingObject(Field* _field, Texture::Kind _kind) : Cell (_kind), field(_field)
{
    vy = 0;
}

void MovingObject::moveLeft()
{
    vx = -v;
}

void MovingObject::moveRight()
{
    vx = v;
}

void MovingObject::stop()
{
    vx = 0;
    vy =0;
}

void MovingObject::moveStep(float delta)
{
    x = x + vx * delta;
    y = y + vy * delta;
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
