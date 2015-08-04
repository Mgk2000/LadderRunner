#include "constructor.h"
#include "logmsg.h"

Constructor::Constructor(View *view) : Field(view)
{
    fillTools();
}

void Constructor::createNewLevel()
{
    this->level = nlevels;
    createEmptyLevel();
}

void Constructor::createEmptyLevel()
{
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
    tools.push_back(new ToolButton(Texture::GOLDEN_KEY));
//    tools.push_back(new ToolButton(Texture::MOVE));
    tools.push_back(new ToolButton(Texture::ZOOM_IN));
    tools.push_back(new ToolButton(Texture::ZOOM_OUT));
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
            if (currTool == Texture::CROSS)
                cell(j,i)->setKind(Texture::EMPTY);
            else
                cell(j,i)->setKind(currTool);
        }
    }
}
