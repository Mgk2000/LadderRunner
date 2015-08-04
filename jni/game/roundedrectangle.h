#ifndef ROUNDEDDRECTANGLE_H
#define ROUNDEDDRECTANGLE_H
#include "rectangle.h"

class RoundedRectangle : public ARectangle{
public:
    RoundedRectangle(View* view, float w, float h);
    void init();
    void initGL();
private:
    virtual void setParams(float x1, float y1, float x2, float y2, const Point4D &color);
    float _width, _height;
};

#endif // ROUNDEDDRECTANGLE_H
