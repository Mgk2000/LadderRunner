#ifndef RUNNERDRAW_H
#define RUNNERDRAW_H
#include "drawingobject.h"
#include "texture.h"
class Runner;
class View;
class RunnerDraw : public DrawingObject
{
public:
    RunnerDraw(View* _view );
    void setRunner(Runner* _runner) {runner = _runner;}
    void draw(float _x, float _y, float _scale);
    void initGL();
    Runner* runner;
    void getVert(int icoord, Point4D* vert, bool mirror);
//    static float rCoords[2][16];
};

#endif // RUNNERDRAW_H
