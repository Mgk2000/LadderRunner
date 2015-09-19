#ifndef RUNNER_H
#define RUNNER_H
#include "movingobject.h"
class Block;
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
    void catchBonus1();
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
    bool onBlockLift() const;
    bool strongClimbing() const;
    bool nearBombBlock() const;
    bool nearLiftBlock() const;
    Block *leftBlock, *topBlock , *rightBlock, *bottomBlock, *inBlock;
    void calcNearBlocks();
    void clearNearBlocks();
    virtual bool checkFall(float delta);


};

#endif // RUNNER_H
