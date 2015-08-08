#include "field.h"
#include "logmsg.h"
#include "view.h"
#include "rectangle.h"
#include "cell.h"
#include <fstream>
#include "runner.h"
#include "math_helper.h"
Field::Field(View *_view) : view(_view), nlevels(0), cellWidth(1.6667 /8),
    left(0), bottom(0), scale(1.0), cellDraw(_view), nToolColumns(1),
    currTool(Texture::EMPTY),cells(0)
{
    nScreenXCells = 16;
    cellWidth = 1.6667 /nScreenXCells;
    toolButtonSwitch = false;
}

const char *Field::dirName()
{
    return view->getDirName();
}

void Field::openLevel(int l)
{
    deleteCells();
    level = l;
    char fn[16];
    sprintf(fn, "/level.%d",level);
    char buf[256];
    strcpy(buf,dirName());
    strcat(buf,fn);
    unsigned short nr , nc;
    std::ifstream f (buf, std::ios::in | std::ios::binary);
    if (f.is_open())
    {
        f.read((char*) &nc, 2);
        f.read((char*) &nr, 2);
        ncols = nc;
        nrows = nr;
        int sz = ncols*nrows;
        unsigned char* levelbuf = new unsigned char[sz];
        f.read((char*) levelbuf, sz);
        cells = new Cell*[ncols * nrows];
        for (int i=0; i< nrows; i++)
            for (int j = 0; j< ncols; j++)
            {
                Texture::Kind kind = (Texture::Kind)levelbuf[j+ i* ncols];
                if (!playing || !canMove(kind))
                    cells[j+ i* ncols] = new Cell(kind);
                else
                {
                    cells[j+ i* ncols] = new Cell();
                    MovingObject* mo = 0;
                    switch (kind)
                    {
                    case Texture::RUNNER:
                        runner = new Runner(this);
                        mo = runner;
                        break;
                    default:
                        break;
                    }
                    if (mo)
                    {
                        mo->setX(j * cellWidth);
                        mo->setY(i * cellWidth);
                        mo->setVX(0);
                        mo->setVY(0);
                    }
                }
            }
        delete[] levelbuf;
    }
    f.close();
    ladderLength = 5;
    ladderLength2 = sqr(ladderLength);
}

Cell *Field::cell(int x, int y) const
{
    return cells [x + y * ncols];
}

void Field::drawField()
{
    rect()->drawFramed(-1.6667 - left* scale, -1 - bottom* scale,
                 -1.6667 - (left - cellWidth * 2 * ncols) * scale,
                 -1 - (bottom - cellWidth * 2* nrows) * scale, 1,
                 Color(0,0.2,0.4), COLOR_YELLOW);
    int b = bottom;
    int l = left;
    int r = l + 1.6667*2.0 / (cellWidth * scale);
    int t = b + 2.0 / (cellWidth * scale);
    if (b < 0) b = 0;
    if (l < 0) l = 0;
    if (t >=nrows) t = nrows-1;
    if (r >= ncols ) r = ncols-1;
    t = nrows-1;
    r = ncols-1;
    for (int i = b; i<= t; i++)
        for (int j = l; j <= r; j++)
        {
            float sc = 1.0;
            float dy  = 0.0;
            switch (cell(j,i)->kind)
            {
            case Texture::GOLDEN_KEY:
                sc = 0.3;
                dy = -cellWidth*0.7;
                break;
            case Texture::BOMB:
                sc = 0.3;
                dy = -cellWidth*0.6;
                break;
            default:
                break;
            }

            float xx = (cellWidth*j*2 - left) * scale -1.6667 + cellWidth*scale ;
            float yy = (cellWidth*i*2 - bottom) * scale -1 + cellWidth*scale;
            cellDraw.draw(cell(j,i), xx, yy + dy * scale, cellWidth * scale * sc);
            //cell(j, i)->draw(xx, yy, scale);
        }

}

void Field::setLevel(int l)
{

}

void Field::drawToolbar()
{
    checkToolButtonSwitch();
   for (int i=0; i< tools.size(); i++)
   {
       if (currTool == tools[i]->kind)
            rect()->draw(tools[i]->left(), tools[i]->bottom(), tools[i]->right(), tools[i]->top(), COLOR_BLUE);
       cellDraw.draw(tools[i]->kind,  tools[i]->x, tools[i]->y, 1.0/16);

   }
}

void Field::setToolButtonsCoords()
{
    float _scale = 0.0625;
    float dx =_scale* 2;// 0.0625;
    float dy =_scale* 2;// 0.0625;
    toolbarLeft =  1.66667 - _scale * nToolColumns - dx * (nToolColumns + 1);
    for (int i =0; i < tools.size(); i++)
    {
        int col = i % nToolColumns;
        int row = i / nToolColumns;
        tools[i]->x = toolbarLeft + dx + col * (dx+_scale) ;
        tools[i]->y = 1 - dy - row * (dy+_scale);
        tools[i]->width = _scale * 2;
    }
}

void Field::screenToField(float x, float y, int *j, int *i)
{
    *j = ((x+1.66667)/scale + left) /cellWidth /2;
    *i = ((y+1  )/scale + bottom) /cellWidth / 2;
    if (*j<0 || *j>=ncols || *i < 0 || *i >=  nrows)
        *j = -1;
}
void Field::fieldToScreen(float fx, float fy, float* sx, float* sy)
{
//    *j = ((x+1.66667)/scale + left) /cellWidth /2;
//    (x+1.66667)/scale + left = *j * cellwidth * 2;
//    (x+1.66667)/scale = *j * cellwidth * 2 - left;
//    x+1.66667 = (*j * cellwidth * 2 - left) * scale;
//    x = (*j * cellwidth * 2 - left) * scale - 1.6667;
    *sx = (fx * cellWidth * 2 - left + cellWidth) * scale - 1.6667;
//    *i = ((y+1  )/scale + bottom) /cellWidth / 2;
//    if (*j<0 || *j>=ncols || *i < 0 || *i >=  nrows)
//        *j = -1;
    *sy = (fy * cellWidth * 2 - bottom + cellWidth) * scale - 1.0;
}

long long Field::currTime() const
{
    return view->getCurrTime();
}

void Field::switchToolButton(Texture::Kind tool)
{
    toolButtonSwitch = true;
    currTool = tool;
    toolButtonSwitchTime = currTime() + 500;
}

void Field::checkToolButtonSwitch()
{
    if (!toolButtonSwitch)
        return;
    if (currTime() < toolButtonSwitchTime)
        return;
    currTool = Texture::EMPTY;
    toolButtonSwitch = false;
}
ARectangle *Field::rect() const
{
    return view->rect;
}

void Field::deleteCells()
{
    if (!cells)
        return;
    for (int i = 0; i< nrows; i++)
        for (int j = 0; j< ncols; j++)
            delete cells[j+ i* ncols];
    delete[] cells;
    ncols = nrows = 0;

}

BitmapText *Field::bitmapText() const
{
    return view->getBitmapText();
}

bool Field::canMove(Texture::Kind _kind) const
{
    switch (_kind)
    {
    case Texture::RUNNER:
        return true;
    default:
        return false;
    }
}
