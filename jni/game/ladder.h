#ifndef LADDER_H
#define LADDER_H
#include "drawingobject.h"
class Play;

class Ladder : public DrawingObject
{
public:
    Ladder(View* _view, Play* _field);
    void setCoords (float _x1, float _y1, float _x2,  float _y2, float __angle);
    void draw();
    void initGL();
    float x1, y1, x2, y2;
    int nl;
    Play* field;
    float angle;
};

#endif // LADDER_H
