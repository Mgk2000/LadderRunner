#include "constructor.h"
#include "logmsg.h"
#include "bitmaptext.h"
#include <math.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

Constructor::Constructor(View *view) : Field(view)
{
    playing = false;
   nToolColumns = 2;
    fillTools();
}

void Constructor::createNewLevel()
{
    this->level = nlevels;
    createEmptyLevel();
}

void Constructor::createEmptyLevel()
{
    deleteCells();
    nrows = 32;
    ncols = 32;
    cells = new Cell*[nrows* ncols];
    for (int i=0;  i< nrows; i++)
    {
        for (int j=0; j< ncols; j++)
        {
            cells[i * ncols + j] = new Cell();
            Cell* c = cell(j,i);
            if (i==0)
            {
                switch(j)
                {
                case 0:
                    c->setKind(Texture::RUNNER);
                    break;
                case 1:
                    c->setKind(Texture::BRICK);
                    break;
                case 2:
                    c->setKind(Texture::SOLID_BRICK);
                    break;
                case 3:
                    c->setKind(Texture::SOFT_BRICK);
                    break;
                case 4:
                    c->setKind(Texture::GOLDEN_KEY);
                    break;
                case 5:
                    c->setKind(Texture::DOOR);
                    break;
                default:
                   c->setKind(Texture::EMPTY);
                }
            }
        }
    }

}

void Constructor::drawFrame()
{
    draw();
}

void Constructor::fillTools()
{
    tools.push_back(new ToolButton(Texture::CROSS));
    tools.push_back(new ToolButton(Texture::RUNNER));
    tools.push_back(new ToolButton(Texture::BRICK));
    tools.push_back(new ToolButton(Texture::SOLID_BRICK));
    tools.push_back(new ToolButton(Texture::SOFT_BRICK));
    tools.push_back(new ToolButton(Texture::BIG_BRICK));
    tools.push_back(new ToolButton(Texture::BIG_SOLID_BRICK));
    tools.push_back(new ToolButton(Texture::BIG_SOFT_BRICK));
    tools.push_back(new ToolButton(Texture::GOLDEN_KEY));
    tools.push_back(new ToolButton(Texture::DOOR));
//    tools.push_back(new ToolButton(Texture::MOVE));
    tools.push_back(new ToolButton(Texture::EXPAND_HOR));
    tools.push_back(new ToolButton(Texture::EXPAND_VERT));
    tools.push_back(new ToolButton(Texture::DECREASE_HOR));
    tools.push_back(new ToolButton(Texture::DECREASE_VERT));
    tools.push_back(new ToolButton(Texture::ZOOM_IN));
    tools.push_back(new ToolButton(Texture::ZOOM_OUT));
    tools.push_back(new ToolButton(Texture::SAVE));
    setToolButtonsCoords();
}

void Constructor::draw()
{
//    LOGD("Constructor::draw currTool=", (int)currTool);
    drawField();
    drawToolbar();
}

void Constructor::processTouchMove(float x, float y)
{
    left = left - (x-lastX) / scale;
    bottom = bottom - (y-lastY) / scale;
    lastX = x;
    lastY = y;
}

void Constructor::processTouchPress(float x, float y)
{
    lastX = x;
    lastY = y;
    if (toolbarPressed(x))
    {
        currTool = Texture::EMPTY;
        for (int i =0; i< tools.size(); i++)
        {
            if (tools[i]->pressed(x, y))
            {
               Texture::Kind tool = tools[i]->kind;
               switch (tool)
               {
               case Texture::ZOOM_IN:
                   if (scale < 0.99)
                     scale = scale * 2;
                   break;
               case  Texture::ZOOM_OUT:
                   if (scale > 1/16 - 0.001)
                     scale = scale * 0.5;
                   break;
               case Texture::EXPAND_HOR:
               case Texture::EXPAND_VERT:
               case Texture::DECREASE_HOR:
               case Texture::DECREASE_VERT:
               {
                   int newncols = ncols, newnrows=nrows;
                   switch(tool)
                   {
                   case Texture::EXPAND_HOR:
                       newncols++;
                       break;
                   case Texture::EXPAND_VERT:
                       newnrows++;
                       break;
                   case Texture::DECREASE_HOR:
                       newncols--;
                       break;
                   case Texture::DECREASE_VERT:
                       newnrows--;
                   }
                   resizeField(newncols, newnrows);
                   break;
               }
               case Texture::SAVE:
                   saveLevel();
                   break;
               default:
                   currTool = tool;
//                   LOGD ("Pressed tool=%d", (int)tools[i]->kind);
                   break;
               }

            }
        }
    }
    else if (currTool != Texture::EMPTY)
    {
        int j, i;
        screenToField(x, y, &j, &i);
        LOGD("Pressed x=%d y=%d", j, i);
        if (j>=0)
        {
            switch (currTool)
            {
            case Texture::CROSS:
                cell(j,i)->setKind(Texture::EMPTY);
                break;
            case Texture::RUNNER:
            {
                for (int ii =0; ii< nrows; ii++)
                    for (int jj = 0; jj< ncols; jj++)
                        if (cell(jj,ii)->kind == Texture::RUNNER)
                            cell(jj,ii)->kind = Texture::EMPTY;
                cell(j,i)->setKind(currTool);
                break;
            }
            default:
                cell(j,i)->setKind(currTool);
            }
        }
    }
}

void Constructor::resizeField(int newNCols, int newNRows)
{
    Cell** newCells = new Cell*[newNCols* newNRows];
    int maxc = std::min (newNCols, ncols);
    int maxr = std::min (newNRows, nrows);
    for (int i = 0; i< newNRows; i++)
        for (int j = 0; j< newNCols; j++)
        {
            Cell* newcell = new Cell;
            if (i<maxr && j< maxc)
                newcell->setKind((Texture::Kind)cells[j+ i*ncols]->kind);
            newCells[i*newNCols + j] = newcell;
        }
    deleteCells();
    cells = newCells;
    nrows = newNRows;
    ncols = newNCols;
}

void Constructor::drawToolbar()
{
    Field::drawToolbar();
    char buf[16];
    sprintf(buf, "%dx%d", ncols, nrows);
    bitmapText()->draw(toolbarLeft,-0.95,0.05,COLOR_YELLOW, buf);
}

void Constructor::saveLevel()
{
    char fn[16];
    sprintf(fn, "/level.%d",level);
    char buf[256];
    strcpy(buf,dirName());
    strcat(buf,fn);
    int sz = 4+ncols*nrows;
    unsigned char* levelbuf = new unsigned char[sz];
    unsigned short nr =nrows;
    unsigned short nc = ncols;
    memcpy(levelbuf,&nc,2);
    memcpy(&levelbuf[2],&nr,2);
    for (int i=0; i<nrows;i++)
        for (int j=0; j< ncols; j++)
            levelbuf[4+ i*ncols+j] = cell(j,i)->kind;
    std::ofstream f (buf, std::ios::out | std::ios::binary);
    if (f.is_open())
        f.write((const char*) levelbuf, sz);
    f.close();
    delete[] levelbuf;
}
