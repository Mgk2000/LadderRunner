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
    void revive();
    void die();
    bool alive;
    bool out() const;
    long long endTime;
    virtual void moveLeft();
    virtual void moveRight();
    virtual void tryMoveLeft(float delta);
    virtual void tryMoveRight(float delta);
    virtual bool canMoveLeft() const;
    virtual bool canMoveRight() const;
    int phase() const;
    int phaseLength;
    long long beginWalkTime;
    bool lastLeft;
    bool armored;
    float postVX;
    virtual void fall();

};

#endif // RUNNER_H
