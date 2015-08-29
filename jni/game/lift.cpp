#include "lift.h"
#include "play.h"
#include "runner.h"
#include <math.h>
#include "block.h"

Lift::Lift(Play* _field) : MovingObject(_field, Texture::LIFT), block(0)
{
    v = 5.0;
}

void Lift::init()
{
    for (int i = y+1; i< field->nrows; i++)
        if (!field->cell(x,i)->free() )
        {
            y2 = i-1;
            break;
        }
        else if (i==field->nrows-1)
        {
            y2 = i;
            break;
        }
    y1 = 0;
    for (int i = y-1; i>= 0; i--)
//        if (field->hasSurface(x,i))
          if (field->isBrick(x,i) || field->blockOfXY(x,i) !=0)
        {
            y1 = i+1;
            break;
        }
        //else if (i ==0)
//            y1=0;

}

Runner *Lift::runner() const
{
    return field->runner;
}

bool Lift::runnerWaiting()
{
    init();
    if (runner()->y > y2 || runner()->y < y1)
        return false;
    if (round(runner()->x) == round(x))
        return false;
    if (runner()->vx !=0 || runner()->vy !=0)
        return false;
    int rx = round(runner()->x);
    if (runner()->onBlockLift())
        return false;
    if (rx >= x-2 && rx <= x+2 )
        return true; //false;
    return false;
    //return true;
}

void Lift::moveToRunner()
{
    if (abs(y-runner()->y) < 0.1)
    {
        y = runner()->y;
        vy = 0;
        return;
    }
    if (runner()->y > y)
        vy = v;
    else if (runner()->y < y)
        vy = -v;
    else vy = 0;
}

void Lift::moveStep(float delta)
{
    MovingObject::moveStep(delta);
    if (runnerWaiting())
        moveToRunner();
    else if (abs(vy)>0.1)
        doStop();

    if (block)
    {
        block->setX(x);
        block->setY(y);
        block->setVX(0);
        block->setVY(vy);
    }
}
