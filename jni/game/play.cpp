#include "play.h"
#include "runner.h"
#include "math.h"
#include "logmsg.h"

Play::Play(View *_view) : Field(_view)
{
    playing = true;
    fillTools();
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
    int j, i;
    screenToField(x, y, &j, &i);
    if (cell(j,i)->kind == Texture::PLACE)
    {
        runner->setX(j);
        runner->setY(i);
        hideLadderHints();
        return;
    }
    hideLadderHints();
//    LOGD("Pressed x=%d y=%d", j, i);

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
    else if (pressedUpMove (x,y))
    {
        showLadderHints();
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

bool Play::isLeftCorner(int x, int y) const
{
    if (x<=1 || x > ncols-1 || y <=0 || y >= nrows-1)
        return false;
    return isBrick(x,y) && !isBrick(x-1,y) && !isBrick(x-1, y+1) && !isBrick(x, y+1);
}

bool Play::isRightCorner(int x, int y) const
{
    if (x < 0 || x >= ncols-1 || y <=0 || y >= nrows-1)
        return false;
    return isBrick(x,y) && !isBrick(x+1,y) && !isBrick(x+1, y+1) && !isBrick(x, y+1);
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
    drawToolbar();
}

void Play::drawMoveables()
{
    float xx, yy;
    fieldToScreen(runner->x, runner->y, &xx, &yy);
    cellDraw.draw(runner, xx, yy, cellWidth*scale);
}

bool Play::pressedRightMove(float x, float y) const
{
    return x > 0 && y < 0.8;
}

bool Play::pressedLeftMove(float x, float y) const
{
    return x < 0 && y < 0.8;
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
    //if (dx< 0.5)
    //    return true;
    return rightFree(xx, yy);
}

void Play::moveStep()
{
    float delta = (currTime() - prevTime) / 1000.0;
    prevTime = currTime();
    if (delta>0.1)
        return;
//    if (runner->vx ==0 && runner->vy == 0)
//        return;
    int xxx = runner->x;
    if (!runnerCanMove())
        runner->stop();
    else
        runner->moveStep(delta);
    int xx = runner->x;
    //if (runner->vx<0 && xx>0)
    //    xx--;
    if (!hasSurface(xx,runner->y))
    {
        runner->x =xx;
        runnerFall();
    }
}

void Play::adjustScreenPosition()
{
    if (runner->x < nScreenXCells*0.5)
        left = 0;
    else if (runner->x > ncols - nScreenXCells*0.5)
        left = (ncols-nScreenXCells) * 2 * cellWidth;
    else
        left = (runner->x - nScreenXCells*0.5) * 2 * cellWidth;
    if (runner->y < 4)
        bottom = 0;
    else if (runner->y > nrows - nScreenXCells*0.5 * 0.6)
        bottom = (nrows-nScreenXCells*0.6) * 2 * cellWidth;
    else
        bottom = (runner->y - nScreenXCells*0.5*0.6) * 2 * cellWidth;
}

void Play::drawToolbar()
{
    Field::drawToolbar();
}

void Play::fillTools()
{
    tools.push_back(new ToolButton(Texture::LADDER));
    tools.push_back(new ToolButton(Texture::BOMB));
    tools.push_back(new ToolButton(Texture::BOMB));
    setToolButtonsCoords();
}

void Play::setToolButtonsCoords()
{
    float _scale = 0.0625;
    float dx =_scale* 2;// 0.0625;
    float dy = 0; //_scale* 2;// 0.0625;
    float toolbarBottom =  1.0 - cellWidth;
    float toolbarLeft = -1.6667;
    for (int i =0; i < tools.size(); i++)
    {
        tools[i]->x = toolbarLeft + dx + i * (dx+_scale) ;
        tools[i]->y = toolbarBottom + (dy+_scale);
        tools[i]->width = _scale * 2;
    }
}

float Play::cellDistance(int x1, int y1, int x2, int y2) const
{
    int dx = x2-x1;
    int dy = y2-y1;
    return (float)(dx*dx + dy*dy);
}

bool Play::canLadder(int x1, int y1, int x2, int y2) const
{
    if (x1 == x2)
        return false;
    if (x1-x2 > ladderLength || x2-x1 > ladderLength || y1-y2 > ladderLength || y2-y1 > ladderLength)
        return false;
    if (cellDistance(x1,y1, x2, y2) > ladderLength2)
        return false;
    if (y1 == y2)
    {
        if (x1 < x2)
        {
            for (int i = x1+1; i<= x2; i++)
                if (isBrick(i, y1))
                    return false;
            return (isRightCorner(x1,y1-1) && isLeftCorner(x2,y2-1));
        }
        else
        {
            for (int i = x2; i< x1; i++)
                if (isBrick(i, y1))
                    return false;
            return (isRightCorner(x2,y2-1) && isLeftCorner(x1,y1-1));
        }
    }
    float dy = y2 -y1;
    float dx = x2 - x1;
    if (y2>y1)
    {
        if (x2>x1)
        {
            if (!isLeftCorner(x2,y2-1))
                return false;
            if (y2-y1 > x2-x1)
            {
                for (int i = y1+1; i < y2; i++)
                {
                    float fxx = x1 + 1.0* dx* (i-y1) / (y2-y1);
                    int xx = fxx;
                    if (xx==x2 && i == y2-1)
                        continue;
                    if (isBrick(xx, i))
                          return false;
                    xx = fxx - 0.5;
                    if (xx==x2 && i == y2-1)
                        continue;
                    if (isBrick(xx, i))
                          return false;
                }
                return true;
            }
            else
            {
//                if (isBrick(x2-2, y2))
//                    return false;
                for (int j = x1+1; j < x2; j++)
                {
                    float fyy = y1 + 1.0 * dy * (j - x1) / (x2-x1);
                    int yy = fyy;
                    if (isBrick(j, yy) || isBrick(j-1, yy) /*|| isBrick(j-2, yy)*/)
                        return false;
                }
                return true;
            }

        }
        else
        {
            if (!isRightCorner(x2,y2-1))
                return false;
            if (y2-y1 > x1-x2)
            {
                for (int i = y1+1; i < y2; i++)
                {
                    float fxx = x1 + 1.0 * dx* (i-y1) / (y2-y1);
                    int xx = fxx+0.5;
                    if (xx==x2 && i == y2-1)
                        continue;
                    if (isBrick(xx, i))
                          return false;
                    xx = fxx + 1.0;
                    if (xx==x2 && i == y2-1)
                        continue;
                    if (isBrick(xx, i))
                          return false;
                }
                return true;
            }
            else
            {
//                if (isBrick(x2+2, y2))
//                    return false;
                for (int j = x2; j < x1; j++)
                {
                    float fyy = y1 + 1.0* dy * (j - x1) / (x2-x1);
                    int yy = fyy;
                    if (isBrick(j, yy) || isBrick(j+1, yy) /*|| isBrick(j+2, yy)*/)
                        return false;
                }
                return true;
            }

        }
    }
    else return false;
}

void Play::showLadderHints()
{
    int xl = runner->x - ladderLength;
    int xr = runner->x + ladderLength;
    int yb = runner->y - ladderLength;
    int yt = runner->y + ladderLength;
    if (xl<0)
        xl = 0;
    if (xr >= ncols)
        xr = ncols -1;
    if (yb < 0)
        yb = 0;
    if (yt>=nrows)
        yt = nrows-1;
    for (int i = yb; i <=yt; i++)
        for (int j = xl; j<=xr; j++)
            if (canLadder(runner->x, runner->y, j,i))
            {
                cell(j,i)->setKind(Texture::PLACE, true);
            }
}

void Play::hideLadderHints()
{
    for (int i = 0; i< nrows; i++)
        for (int j = 0; j < ncols; j++)
            if (cell(j,i)->kind == Texture::PLACE)
                cell(j,i)->restoreKind();
}

void Play::runnerFall()
{
    for(int i = runner->y-1; i>=0; i--)
    {
        if (hasSurface(runner->x, i))
        {
            runner->y = i;
            return;
        }
    }
}

bool Play::runnerCanMove()
{
    if (runner->vx < 0)
    {
        if (!canMoveLeft(runner->x, runner->y))
        return false;
    }
    else if (runner->vx >0)
    {
        if (!canMoveRight(runner->x, runner->y))
        return false;
    }
    return true;
}

bool Play::hasSurface(int x, int y) const
{
    if (!cell(x, y)->free())
        return false;
    if ( y==0)
        return true;
    if (cell(x, y-1)->free())
        return false;
    return true;
}

bool Play::isBrick(int x, int y) const
{
    switch (cell(x,y)->kind)
    {
    case Texture::BRICK:
    case Texture::BIG_BRICK:
    case Texture::SOLID_BRICK:
    case Texture::BIG_SOLID_BRICK:
    case Texture::SOFT_BRICK:
    case Texture::BIG_SOFT_BRICK:
        return true;
    default:
        return false;
    }
}
