#ifndef PLAY_H
#define PLAY_H
#include "field.h"
class Play : public Field
{
public:
    Play(View* _view);
    void drawFrame();
    virtual void processTouchMove(float x, float y) {}
    virtual void processTouchPress(float x, float y) {}
 };

#endif // PLAY_H
