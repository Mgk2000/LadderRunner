#ifndef BOMB_H
#define BOMB_H

#include "drawingobject.h"
#include "texture.h"

class Play;
class Bomb : public DrawingObject
{
public:
    Bomb(View* _view, Play* _field, Texture* _texture);
    void draw();
    void initGL();
    void moveStep(float delta);
    int phase;
    Play* field;
    long long beginTime, lastTime, endTime;
    int ix, iy;
    void setX(int _ix) {ix = _ix;}
    void setY(int _iy) {iy = _iy;}
    bool out() const;
};

#endif // BOMB_H
