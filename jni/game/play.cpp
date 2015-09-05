#include "play.h"
#include "runner.h"
#include "math.h"
#include "logmsg.h"
#include "bitmaptext.h"
#include "bomb.h"
#include "view.h"
#include "math_helper.h"
#include "explosion.h"
#include "rectangle.h"
#include "block.h"
#include "growingcell.h"
#include "lift.h"

Play::Play(View *_view) : Field(_view), ladder(_view, this)
{
   // playing = true;
    fillTools();
    bombBarLeft = 1.05;
    bombBarBottom = 0.9;
    bombBarWidth=0.1;
    grenadeBarLeft = 1.4;
    grenadeBarBottom = 0.9;
    grenadeBarWidth=0.1;
    explosionRadius = 3.9;
    grenadePutX = -1000;
    explosionRadius2 = sqr(explosionRadius);
}

void Play::drawFrame()
{
    if (playing)
    {
        moveStep();
        adjustScreenPosition();
    }
    draw();
    if (levelDone)
        drawLevelDone();

}

/*void Play::openLevel(int l)
{
    playing = true;
    Field::openLevel(l);
    if (lifts.size())
    {
        std::list<Lift*>::iterator lit = lifts.begin();
        for (; lit != lifts.end(); lit++)
            (*lit)->init();
    }
    prevTime = currTime();
    nRunnerKeys = 0;
    nRunnerBombs = 0;
    levelDone = false;
}*/

void Play::openLevel(int l, const char *buf)
{
    playing = true;
    Field::openLevel(l, buf);
    if (lifts.size())
    {
        std::list<Lift*>::iterator lit = lifts.begin();
        for (; lit != lifts.end(); lit++)
            (*lit)->init();
    }
    nRunnerKeys = 0;
    nRunnerBombs = 0;
    nRunnerGrenades = 0;
    levelDone = false;
    playing = true;
    undo.init(this);
    firstStep = true;
    scale = 0.5;
    topY = topY+2;
//    view->resizeGL(view->width, view->height);
//    showMainDialog();
}

void Play::processTouchPress(float x, float y)
{
   if (toolbarPressed(x,y))
   {
       processToolbarPress(x,y);
       return;
   }
   if ((nRunnerBombs>0 || runner->nearBombBlock()) &&
        (x>= bombBarLeft - bombBarWidth) &&
        (x<= bombBarLeft + bombBarWidth) &&
        (y>= bombBarBottom - bombBarWidth) &&
        (y <= bombBarBottom + bombBarWidth))
   {
       if (!bombs.size() || runner->nearBombBlock())
           undo.save();
       if (runner->nearBombBlock())
       {
           if (runner->leftBlock && runner->leftBlock->bombed())
           {
               fireBombBlock(runner->leftBlock);
               runner->leftBlock = 0;
           }
           if (runner->rightBlock && runner->rightBlock->bombed())
           {
               fireBombBlock(runner->rightBlock);
               runner->rightBlock = 0;
           }
           if (runner->topBlock && runner->topBlock->bombed())
           {
               fireBombBlock(runner->topBlock);
               runner->topBlock = 0;
           }
           if (runner->bottomBlock && runner->bottomBlock->bombed())
           {
               fireBombBlock(runner->bottomBlock);
               runner->bottomBlock = 0;
           }
           if (runner->inBlock && runner->inBlock->bombed())
           {
               fireBombBlock(runner->inBlock);
               runner->inBlock = 0;
           }
       }
       else
       {
           Bomb* bomb = new Bomb(view, this, view->textures[Texture::BURNING_BOMB]);
           bomb->setX(runner->x);
           bomb->setY(runner->y);
           bombs.push_back(bomb);
           nRunnerBombs--;
       }
       return;
   }
   if ((nRunnerGrenades>0) &&
        (x>= grenadeBarLeft - grenadeBarWidth) &&
        (x<= grenadeBarLeft + grenadeBarWidth) &&
        (y>= grenadeBarBottom - grenadeBarWidth) &&
        (y <=grenadeBarBottom + grenadeBarWidth))
   {
       cell(runner->x,runner->y)->setKind(Texture::GRENADE);
       nRunnerGrenades--;
       grenadePutX = round(runner->x);
       grenadePutY = round(runner->y);
       return;
   }
    int j, i;
    screenToField(x, y, &j, &i);
    if (insideField(j,i))
    {
        if (cell(j,i)->kind() == Texture::PLACE)
        {
            runner->climb(j,i);
            //runner->setX(j);
            //runner->setY(i);
            hideLadderHints();
            return;
        }
        hideLadderHints();
//        if (runner->y > 0 && isLeftCorner(runner->x, runner->y-1) && j<runner->x && i<runner->y)
//        {
//            runner->moveLeft();
//            return;
//        }
        if (!runner->busy() || !runner->strongClimbing())
        {
            if (this->canLadder(runner->x, runner->y, j, i))
            {
                runner->climb(j,i);
                //runner->setX(j);
                //runner->setY(i);
                return;
            }
        for (int ii = i-1; ii <= i+1; ii++)
            for (int jj = j-1; jj <=j+1; jj++)
                if (i != ii || j != jj)
                if (this->canLadder(runner->x, runner->y, jj, ii))
                {
                    runner->climb(jj,ii);
                    //runner->setX(j);
                    //runner->setY(i);
                    return;
                }
        }
    //    LOGD("Pressed x=%d y=%d", j, i);
    }
    if (j < runner->x /*|| pressedLeftMove (x,y)*/)
    {
        if (!runner->busy())
        if (runner->canMoveLeft())
        {
            float dx = runner->x - j;
            this->pressNearLeft = (dx>0 && dx <=1.1);
            runner->moveLeft();
        }
    }
    else if (j > runner->x /*|| pressedRightMove (x,y)*/)
    {
        if (!runner->busy())
        if (runner->canMoveRight())
        {
            float dx = j -  runner->x - j;
            runner->moveRight();
            this->pressNearRight = (dx>0 && dx <=1.1);
        }
    }
    //else if (pressedUpMove (x,y))
    //{
    //    showLadderHints();
    //}
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
    if (x<=0 || x > ncols-1 || y <0 || y >= nrows-1)
        return false;
//    Cell* cell = this->cell(x,y);
//    if (cell->growing() && ((GrowingCell*)cell)->solid())
//        return false;
    Lift* lift = liftOfXY(x-1,y+1);
    if (lift)
        return false;
    return isBrick(x,y) && !isBrick(x-1,y) && !isBrick(x-1, y+1) && !isBrick(x, y+1);
}

bool Play::isRightCorner(int x, int y) const
{
    if (x < 0 || x >= ncols-1 || y <0 || y >= nrows-1)
        return false;
//    Cell* cell = this->cell(x,y);
//    if (cell->growing() && ((GrowingCell*)cell)->solid())
//        return false;
    Lift* lift = liftOfXY(x+1,y+1);
    if (lift)
        return false;
    return isBrick(x,y) && !isBrick(x+1,y) && !isBrick(x+1, y+1) && !isBrick(x, y+1);
}


void Play::draw()
{
    drawField();
    for (int i =0; i< nrows; i++)
        for (int j =0; j< ncols; j++)
            if (cell(j, i)->growing())
            {\
                //if (j==5 && i ==2)
                //    LOGD("GROWing");
                GrowingCell* gc = (GrowingCell*) cell(j, i);
                if (gc->out())
                    if (blockOfXY(j, i) != 0)
                {
                    delete gc;
                    cells[j+ncols * i] = new Cell(Texture::EMPTY);
                }
            }
    drawMoveables();
    drawToolbar();
}

void Play::drawMoveables()
{
    float xx, yy;
    fieldToScreen(runner->x, runner->y, &xx, &yy);
    //cellDraw.draw(runner, xx, yy, cellWidth*scale);
    if (runner->climbing)
        ladder.draw();
    runnerDraw.draw(xx, yy, cellWidth*scale);
    std::list<Bomb*>::iterator bit = bombs.begin();
    for (; bit != bombs.end(); bit++)
        (*bit)->draw();
    std::list<Explosion*>::iterator eit = explosions.begin();
    for (; eit != explosions.end(); eit++)
        (*eit)->draw();
    std::list<Block*>::iterator it = blocks.begin();
    for (; it != blocks.end(); it++)
    {
        Block* bl = *it;
        if (bl->x > 999)
            continue;
        Point4D colorm;
        if (!bl->marked)
            colorm = Point4D(1,1,1,1);
        else
            colorm = Point4D(1,0,0,1);
        fieldToScreen(bl->x, bl->y, &xx, &yy);
        cellDraw.draw(bl, xx, yy ,cellWidth*scale, colorm);
    }
    std::list<Lift*>::iterator lit = lifts.begin();
    for (; lit != lifts.end(); lit++)
    {
        Lift* l = *lit;
        fieldToScreen(l->x, l->y, &xx, &yy);
        cellDraw.draw(l, xx, yy ,cellWidth*scale);
    }
    if (!runner->alive)
        drawYouDead();
}

bool Play::pressedRightMove(float x, float y) const
{
    return x > 0 && y < 0.8;
//    this->fi
 //   (return x > runner->x || x>1.) && y < 0.8;
}

bool Play::pressedLeftMove(float x, float y) const
{
    return x < 0 && y < 0.8;
}

bool Play::pressedUpMove(float x, float y) const
{
    return y>0;
}


void Play::moveStep()
{
    if (firstStep)
    {
        runner->calcNearBlocks();;
        firstStep = false;
    }
    float delta = (currTime() - prevTime) / 1000.0;
    prevTime = currTime();
    if (delta>0.1)
        return;
    if (!runner->alive)
    {
        if (runner->out())
            this->restart();
        return;
    }
    for (std::list<Block*>::iterator bit = blocks.begin(); bit != blocks.end(); bit++)
        if ((*bit)->falling)
        (*bit)->moveStep(delta);
    runner->moveStep(delta);
    for (std::list<Block*>::iterator bit = blocks.begin(); bit != blocks.end(); bit++)
        if (!(*bit)->falling)
        (*bit)->moveStep(delta);
    std::list<Bomb*>::iterator bit = bombs.begin();
    std::list<Bomb*>explosedBombs;
    for (; bit != bombs.end(); bit++)
    {
        Bomb* bomb = *bit;
        bomb->moveStep(delta);
        if (bomb->out())
        {
            doExplosion(bomb->ix, bomb->iy, &explosedBombs);
            delete bomb;
            bit = bombs.erase(bit);
        }
    }
    if (explosedBombs.size())
    {
        std::list<Bomb*>::iterator bit = explosedBombs.begin();
        for (;bit != explosedBombs.end(); bit++)
            bombs.push_back(*bit);
        explosedBombs.clear();
    }
    std::list<Explosion*>::iterator eit = explosions.begin();
    for (; eit != explosions.end(); eit++)
    {
        Explosion* expl = *eit;
        expl->moveStep(delta);
        if (expl->out())
        {
            delete expl;
            eit = explosions.erase(eit);
        }
    }
    for (std::list<Lift*>::iterator lit = lifts.begin(); lit != lifts.end(); lit++)
            (*lit)->moveStep(delta);
    runner->calcNearBlocks();;
}

void Play::adjustScreenPosition()
{
    if (!runner || runner->climbing)
        return;
    if (runner->x < nScreenXCells/scale*0.5)
        left = 0;
    else if (runner->x > ncols - nScreenXCells /scale *0.5)
        left = (ncols-nScreenXCells /scale) * 2 * cellWidth;
    else
        left = (runner->x - nScreenXCells/scale*0.5) * 2 * cellWidth;
    if (nScreenXCells/scale * 0.5 >= topY+1)
        bottom = 0;
    else if (runner->y < nScreenXCells/scale*0.5 -2)
        bottom = 0;
    else if (runner->y > topY - nScreenXCells/scale*0.5 * 0.6)
        bottom = (/*nrows*/ topY + 3 -nScreenXCells/scale*0.6) * 2 * cellWidth;
    else
        bottom = (runner->y - nScreenXCells/scale*0.5*0.4) * 2 * cellWidth;
//        bottom = (runner->y - topY + 2) * 2 * cellWidth;
    if (bottom<0)
        bottom = 0;
}

bool Play::toolEnabled(Texture::Kind kind) const
{
    switch(kind)
    {
    case Texture::UNDO:
        return undo.canRestore();
    default:
        return Field::toolEnabled(kind);
    }
}

void Play::drawToolbar()
{
    rect()->draw(toolbarLeft, toolbarBottom, toolbarRight, toolbarTop, Point4D(0.75, 0.75, 0.75, 0.3));
    Field::drawToolbar();
    if (runner->nearBombBlock())
        cellDraw.draw(Texture::BOMB_BLOCK,  bombBarLeft, bombBarBottom, 1.0/8);
    else if (nRunnerBombs >0)
    {
        cellDraw.draw(Texture::BOMB,  bombBarLeft, bombBarBottom, 1.0/8);
        char buf[8];
        sprintf (buf, "%d", nRunnerBombs);
        bitmapText()->draw(bombBarLeft+0.15, bombBarBottom-0.05, 0.07, COLOR_YELLOW, buf);
    }
    if (nRunnerGrenades)
    {
        cellDraw.draw(Texture::GRENADE,  grenadeBarLeft, grenadeBarBottom, 1.0/8);
        char buf[8];
        sprintf (buf, "%d", nRunnerGrenades);
        bitmapText()->draw(grenadeBarLeft+0.15, grenadeBarBottom-0.05, 0.07, COLOR_YELLOW, buf);

    }

}

void Play::fillTools()
{
    tools.push_back(new ToolButton(Texture::RESTART));
    tools.push_back(new ToolButton(Texture::EMPTY));
    tools.push_back(new ToolButton(Texture::ZOOM_IN));
    tools.push_back(new ToolButton(Texture::ZOOM_OUT));
    tools.push_back(new ToolButton(Texture::EMPTY));
    tools.push_back(new ToolButton(Texture::UNDO));
    tools.push_back(new ToolButton(Texture::EMPTY));
    tools.push_back(new ToolButton(Texture::EXIT));

    setToolButtonsCoords();
}

void Play::setToolButtonsCoords()
{
    float _scale = 0.0625;
    float dx =_scale* 2;// 0.0625;
    float dy = _scale* 1;// 0.0625;
    toolbarBottom =  1.0 - cellWidth - dy*2;
    toolbarLeft = -1.6667;
    toolbarTop = 1.1;
    for (int i =0; i < tools.size(); i++)
    {
        tools[i]->x = toolbarLeft + dx + i * (dx+_scale) ;
        tools[i]->y = toolbarBottom + (dy+_scale);
        tools[i]->width = _scale * 2;
    }
    toolbarRight = toolbarLeft + tools.size() * (dx + _scale) + dx;
}

float Play::cellDistance(int x1, int y1, int x2, int y2) const
{
    int dx = x2-x1;
    int dy = y2-y1;
    return (float)(dx*dx + dy*dy);
}

bool Play::canLadder(int x1, int y1, int x2, int y2) const
{
    if (x2<0 || x2 >= ncols || y2 <0 || y2 >=nrows)
        return false;
    if (x1 == x2 && cell(x2,y2)->kind() != Texture::OPEN_DOOR)
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
//            return (isRightCorner(x1,y1-1) && isLeftCorner(x2,y2-1));
            return isLeftCorner(x2,y2-1);
        }
        else
        {
            for (int i = x2; i< x1; i++)
                if (isBrick(i, y1))
                    return false;
            //return (isRightCorner(x2,y2-1) && isLeftCorner(x1,y1-1));
            return isRightCorner(x2,y2-1);// && isLeftCorner(x1,y1-1));
        }
    }
    float dy = y2 -y1;
    float dx = x2 - x1;
    if (y2>y1)
    {
        if (x2>x1)
        {
            dx = dx - 0.5;
            if (!isLeftCorner(x2,y2-1) && cell(x2,y2)->kind() != Texture::OPEN_DOOR)
                return false;
            if (y2-y1 > x2-x1)
            {
                for (int i = y1+1; i < y2; i++)
                {
                    float fxx = x1 + 1.0* dx* (i-y1) / (y2-y1);
                    int xx = round(fxx);
                    if (xx==x2 && i == y2-1)
                        continue;
                    if (isBrick(xx, i))
                          return false;
                    xx = fxx - 0.5;
                    if (xx< x1)
                        continue;
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
                    if (isBrick(j, yy) || isBrick(j-1, yy) /*|| isBlock(j-2, yy)*/)
                        return false;
                }
                return true;
            }

        }
        else
        {
            if (x1 != x2)
                dx = dx + 0.5;
            if (!isRightCorner(x2,y2-1) && cell(x2,y2)->kind() != Texture::OPEN_DOOR)
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
                    if (x1 !=x2)
                    {
                        xx = fxx + 1.0;
                        if (xx==x2 && i == y2-1)
                            continue;
                        if (isBrick(xx, i))
                              return false;
                    }
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
            if (cell(j,i)->kind() == Texture::PLACE)
                cell(j,i)->restoreKind();
}

void Play::openDoor()
{
    for (int i =0; i< nrows; i++)
        for (int j = 0; j< ncols; j++)
        {
            if (cell(j,i)->kind() == Texture::DOOR)
                cell(j,i)->setKind(Texture::OPEN_DOOR);
        }
}

void Play::doLevelDone()
{
    playing = false;
    levelDone = true;
}

void Play::drawLevelDone()
{
    this->bitmapText()->drawCenter(0,0, 0.1, COLOR_YELLOW, "Level done!");
}

void Play::drawYouDead()
{
    this->bitmapText()->drawCenter(0,0, 0.1, COLOR_RED, "You are dead...");
}

bool Play::toolbarPressed(float x, float y) const
{
    return isInsideRect(x, y, toolbarLeft, toolbarBottom, toolbarRight, toolbarTop );
}

void Play::processToolbarPress(float x, float y)
{
    currTool = Texture::EMPTY;
    for (int i =0; i< tools.size(); i++)
    {
        if (tools[i]->pressed(x, y))
        {
           Texture::Kind tool = tools[i]->kind;
           switch (tool)
           {
            case Texture::RESTART:
               this->restart();
               break;
           case Texture::ZOOM_IN:
               if (scale < 0.99)
                 scale = scale * 2;
               break;
           case  Texture::ZOOM_OUT:
               if (scale > 1/16 - 0.001)
                 scale = scale * 0.5;
               break;
           case Texture::UNDO:
               if (undo.canRestore())
                   undo.restore();
               break;
           case Texture::EXIT:
               view->showMainDialog();
               break;
           default: break;
           }
        }
    }
}

void Play::doExplosion(float bx, float by, std::list<Bomb*>* explosedBombs)
{
    for (int i = by - explosionRadius; i< by + explosionRadius; i++ )
        for (int j = bx - explosionRadius; j< bx + explosionRadius; j++ )
            if (i>=0 && i<nrows && j >=0 && j<ncols)
                if (dist2(bx, by, j,i) <=explosionRadius2)
                {
                    if (cell(j,i)->breakable())
                    {
                        Texture::Kind cellKind = (Texture::Kind) cell(j, i)->_kind;
                        switch (cellKind)
                        {
                        case Texture::BIG_BRICK:
                        case Texture::BRICK:
                        {
                            int ind = j + i*ncols;
                            delete cells[ind];
                            cells[ind] = new GrowingCell(this);
                            cells[ind]->setKind(cellKind);
                            break;
                        }
                        default:
                            cell(j,i)->setKind(Texture::EMPTY);
                        }
                    }
                    else if (cell(j,i)->kind() == Texture::BOMB || cell(j,i)->kind() == Texture::GRENADE)
                    {
                        Bomb* b = new Bomb(view, this, view->textures[Texture::BURNING_BOMB]);
                        b->setX(j);
                        b->setY(i);
                        explosedBombs->push_back(b);
                        cell(j,i)->setKind(Texture::EMPTY);

                    }
                }
    bool blockFired = true;
    while (blockFired)
    {
        blockFired = false;
        std::list <Block*>::iterator bit = blocks.begin();
        for (; bit != blocks.end(); bit++)
        {
            Block* block = *bit;
            if (block->bombed())
            if (dist2(bx, by, block->x ,block->y) <=explosionRadius2)
            {
                fireBombBlock(block);
                blockFired = true;
                break;
            }
            if (block->lifted())
            if (dist2(bx, by, block->x ,block->y) <=explosionRadius2)
            {
                Lift* lift = new Lift(this);
                lift->setX(block->x);
                lift->setY(block->y);
                lifts.push_back(lift);
                bit = blocks.erase((bit));
                delete block;
                blockFired = true;
                break;
            }
        }
    }
    Explosion * expl = new Explosion(view, this, 0.5);
    expl->setX(bx);
    expl->setY(by);
    explosions.push_back(expl);
    if (!runner->armored)
    if (dist2(bx, by, runner->x, runner->y) <= explosionRadius2)
        runner->die();
}

void Play::clearBombs()
{
    if (bombs.size())
    {
        std::list<Bomb*>::iterator bit = bombs.begin();
        for (; bit != bombs.end(); bit++)
            delete *bit;
        bombs.clear();
    }
}

void Play::showMainDialog()
{
    view->showMainDialog();
}

void Play::fireBombBlock(Block *block)
{
    Bomb* bomb = new Bomb(view, this, view->textures[Texture::BURNING_BOMB]);
    bomb->setX(block->x);
    bomb->setY(block->y);
    bombs.push_back(bomb);
    /*std::list<Block*>::iterator bit = blocks.begin();
    for(; bit != blocks.end(); bit++)
        if (*bit == block)
        {
            bit = blocks.erase(bit);
            break;
        }
    delete block;*/
    block->setX(1000);
}

void Play::clearLevel()
{
    if (runner)
    {
        delete runner;
        runner = 0;
    }
    clearBombs();
    if (blocks.size())
    {
        std::list<Block*>::iterator bit = blocks.begin();
        for (; bit != blocks.end(); bit++)
            delete *bit;
        blocks.clear();
    }
    if (explosions.size())
    {
        std::list<Explosion*>::iterator bit = explosions.begin();
        for (; bit != explosions.end(); bit++)
            delete *bit;
        explosions.clear();
    }
    if (lifts.size())
    {
        std::list<Lift*>::iterator bit = lifts.begin();
        for (; bit != lifts.end(); bit++)
            delete *bit;
        lifts.clear();
    }
}

bool Play::hasSurface(int x, int y) const
{
    if (!cell(x, y)->free())
        return false;
    if ( y==0)
        return true;
//    if (cell(x, y-1)->free())
    for (std::list<Lift*>::const_iterator lit = lifts.begin();
         lit != lifts.end(); lit++)
    {
        Lift* lift = *lit;
        if ((int) round(lift->x) == x && (int) round(lift->y) == y)
            return true;
    }
    if (isBrick(x,y-1))
        return true;
    Block * block = blockOfXY(x, y-1);
    if ( block!= 0 && block->vy > -4 )
        return true;
    return false;
}

bool Play::isBrick(int x, int y, bool exact) const
{
    switch (cell(x,y)->kind())
    {
    case Texture::BRICK:
    case Texture::BIG_BRICK:
    case Texture::SOLID_BRICK:
    case Texture::BIG_SOLID_BRICK:
    case Texture::SOFT_BRICK:
    case Texture::BIG_SOFT_BRICK:
        return true;
    default:
        if (exact)
           return false;
        return isBlock(x,y);
    }
}

bool Play::isBlock(int x, int y) const
{
    std::list<Block*>::const_iterator bit = blocks.begin();
    for (; bit != blocks.end(); bit++)
        if (x==(*bit)->x && (*bit)->y == y)
            return true;
    return false;
}
bool Play::isHanging(int x, int y) const
{
    if (y==0)
        return false;
    if (x>0 && isBrick(x-1,y))
        return false;
    if (x<ncols-1 && isBrick(x+1,y))
        return false;
    if (isBrick(x,y-1))
        return false;
    if (y < nrows-1 && isBrick(x,y+1))
        return false;
    return true;
}
bool Play::canMoveTo(int x, int y) const
{
    if (x<0 || x >=ncols || y <0 || y >= nrows)
        return false;
    //return cell(x,y)->free();
    return !isBrick(x, y);
}

Block *Play::blockOfXY(int x, int y) const
{
    //LOGD ("blockOfXY x=%d, y=%d", x, y);
    if (y<0 || y >=nrows)
        return 0;
    for (std::list<Block*>::const_iterator bit = blocks.begin(); bit != blocks.end(); bit++)
    {
        Block* block = *bit;
        if ((int) round(block->x) == x && (int) round(block->y) == y)
            return block;
    }
    return 0;
}

Lift *Play::liftOfXY(int x, int y) const
{
    for (std::list<Lift*>::const_iterator lit = lifts.begin(); lit != lifts.end(); lit++)
    {
        Lift* lift = *lit;
        if ((int) round(lift->x) == x && (int) round(lift->y) == y)
            return lift;
    }
    return 0;

}
