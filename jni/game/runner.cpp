#include "runner.h"
#include "math.h"
#include "math_helper.h"

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
    if (dist2(x,y, climbX,climbY) < 0.01)
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
    if (climbing)
    {
        x = x + vx*delta;
        y = y + vy* delta;
    }
    else
        MovingObject::moveStep((delta));
}
