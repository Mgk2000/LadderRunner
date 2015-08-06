#ifndef FIELD_H
#define FIELD_H
#include <globject.h>
#include <vector>
#include "cell.h"
#include "drawingobject.h"

class View;
class ARectangle;
class Runner;
struct ToolButton
{
    ToolButton(Texture::Kind _kind) : kind(_kind) {}
    Texture::Kind kind;
    float x, y, width;
    bool pressed (float _x, float _y) const {return _x>=x-width && _x <= x + width && _y>=y- width && _y <= y+width;}
    float left() const {return x - width;}
    float top() const {return y + width;}
    float bottom() const {return y - width;}
    float right() const {return x + width;}
};

class Field : public GlObject
{
public:
    Field(View* _view);
    virtual void drawFrame() = 0;
    virtual void processTouchMove(float x, float y) =0;
    virtual void processTouchPress(float x, float y) =0;
    virtual void processTouchRelease(float x, float y){}
    const char* dirName();
    virtual void openLevel(int l);
    int ncols, nrows;
protected:
    Cell** cells;
    Cell* cell(int x , int y) const;
    void drawField();
    int level;
    int nlevels;
    virtual void setLevel(int l);
    int runnerX, runnerY;
    View* view;
    float left, bottom, scale;
    float lastX, lastY;
    float cellWidth;
    CellDraw cellDraw;
    std::vector<ToolButton*> tools;
    Texture::Kind currTool;
    int nToolColumns;
    virtual void fillTools(){}
    virtual void drawToolbar();
    float toolbarLeft;
    bool toolbarPressed(float x) const {return x > toolbarLeft;}
    void setToolButtonsCoords();
    void screenToField(float x, float y, int* i, int*j);
    ARectangle* rect() const;
    void deleteCells();
    BitmapText* bitmapText() const;
    float ladderLength;
    bool playing;
    bool canMove(Texture::Kind _kind) const;
    Runner * runner;
    void fieldToScreen(float fx, float fy, float *sx, float *sy);
    long long currTime() const;
};


#endif // FIELD_H
