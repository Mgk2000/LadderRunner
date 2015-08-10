#ifndef RUNNER_H
#define RUNNER_H
#include "movingobject.h"
class Runner : public MovingObject
{
public:
    Runner(Play* _field);
    float climbX, climbY;
    void climb(int _x, int _y);
    void checkClimb();
    virtual bool busy() const;
    virtual void  moveStep(float delta);
    bool climbing;
    void catchBonus();
    void doStop();
    void stop();

};

#endif // RUNNER_H
