#include "field.h"
#include "logmsg.h"
#include "view.h"
#include "rectangle.h"
Field::Field(View *_view) : view(_view), nlevels(0), cellWidth(1.0 /8),
    left(0), bottom(0), scale(1.0), cellDraw(_view), nToolColumns(1), currTool(Texture::EMPTY)
{
}

Cell *Field::cell(int x, int y)
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
    for (int i = b; i<= t; i++)
        for (int j = l; j <= r; j++)
        {
            float xx = (cellWidth*j*2 - left) * scale -1.6667 + cellWidth*scale ;
            float yy = (cellWidth*i*2 - bottom) * scale -1 + cellWidth*scale;
            cellDraw.draw(cell(j,i), xx, yy, cellWidth*scale);
            //cell(j, i)->draw(xx, yy, scale);
        }

}

void Field::loadLevel()
{

}

void Field::setLevel(int l)
{

}

void Field::drawToolbar()
{
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
    float dx =_scale;// 0.0625;
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
ARectangle *Field::rect() const
{
    return view->rect;
}
