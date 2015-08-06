#include "play.h"
#include "runner.h"

Play::Play(View *_view) : Field(_view)
{
    playing = true;
}

void Play::drawFrame()
{

    moveStep();
    adjustScreenPosition();
    draw();
}

void Play::openLevel(int l)
{
    Field::openLevel(l);
    prevTime = currTime();
}

void Play::processTouchPress(float x, float y)
{
    //float fx, fy;
    //screenToField(x, y, fx, fy);
    if (pressedLeftMove (x,y))
    {
        if (canMoveLeft(runner->x, runner->y))
            runner->moveLeft();
    }
    else if (pressedRightMove (x,y))
    {
        if (this->canMoveRight(runner->x, runner->y))
            runner->moveRight();
    }
}

void Play::processTouchRelease(float x, float y)
{
    runner->stop();
}

bool Play::isCorner(int x, int y) const
{
    if (!hasSurface(x, y))
        return false;
    bool bl = x>0 && (!hasSurface(x-1, y));
    bool br = x< ncols-1 && !hasSurface(x+1, y);
    return (int) bl + (int) br == 1;
}

bool Play::leftFree(int x, int y) const
{
    if (x <=0)
        return false;
    return cell(x-1,y)->free();
}

bool Play::rightFree(int x, int y) const
{
    if (x >= ncols -1)
        return false;
    return cell(x+1,y)->free();

}

void Play::draw()
{
    drawField();
    drawMoveables();
}

void Play::drawMoveables()
{
    float xx, yy;
    fieldToScreen(runner->x, runner->y, &xx, &yy);
    cellDraw.draw(runner, xx, yy, cellWidth*scale);
}

bool Play::pressedRightMove(float x, float y) const
{
    return x > 0 && y < 0;
}

bool Play::pressedLeftMove(float x, float y) const
{
    return x < 0 && y < 0;
}

bool Play::pressedUpMove(float x, float y) const
{
    return y>0;
}

bool Play::canMoveLeft(float x, float y) const
{
    int xx = x ;
    int yy = y;
    float dx = x - xx;
    if (dx> 0.5)
        return true;
    return leftFree(xx, yy);
}

bool Play::canMoveRight(float x, float y) const
{
    int xx = x  ;
    int yy = y ;
    float dx = x - xx ;
    if (dx< 0.5)
        return true;
    return rightFree(xx, yy);
}

void Play::moveStep()
{
    float delta = (currTime() - prevTime) / 1000.0;
    prevTime = currTime();
    if (delta>0.1)
        return;
    runner->moveStep(delta);
}

void Play::adjustScreenPosition()
{
    if (runner->x > nScreenXCells*0.5 && runner->x < ncols - nScreenXCells*0.5)
        left = (runner->x - nScreenXCells*0.5)*0.25;
}

bool Play::hasSurface(int x, int y) const
{
    if (!cell(x, y)->free())
        return false;
    if ( y==0)
        return true;
    if (cell(x, y-1)->free())
        return false;
}
