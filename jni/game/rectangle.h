#ifndef RECTANGLE_H
#define RECTANGLE_H
#include "drawingobject.h"

class ARectangle : public DrawingObject
{
public:
    ARectangle(View* view);
    void init();
    void initGL();
    virtual void draw(float x1, float y1, float x2, float y2, const Point4D& color);
    void drawFrame(float x1, float y1, float x2, float y2, float lwidth, const Point4D& color);
    void drawFramed(float x1, float y1, float x2, float y2, float lwidth,
                    const Point4D& color, const Point4D& framecolor);
protected:
    virtual void setParams(float x1, float y1, float x2, float y2, const Point4D& color);
    float _linewidth;
};

#endif // RECTANGLE_H
