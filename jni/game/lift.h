#ifndef LIFT_H
#define LIFT_H
#include "movingobject.h"
class Play;
class Block;
class Runner;
class Lift : public MovingObject
{
public:
    Lift(Play* field);
    void init();
    int y1, y2;
    Block* block;
    Runner* runner() const;
    bool runnerWaiting();
    void moveToRunner();
    void moveStep(float delta);
    void doStop();
};

#endif // LIFT_H
