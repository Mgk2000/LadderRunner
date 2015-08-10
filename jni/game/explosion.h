#ifndef EXPLOSION_H
#define EXPLOSION_H
#include "drawingobject.h"
class View;
class Play;
class Explosion : public DrawingObject
{
public:
    Explosion(View* view, Play* _field, float __radius);
    bool out() const;
    void draw();
    void setX(int _ix) {ix = _ix;}
    void setY(int _iy) {iy = _iy;}
private:
    float _radius;
    Play* field;
    long long _endTime;
    void initGL();
    int ix, iy;

};

#endif // EXPLOSION_H
