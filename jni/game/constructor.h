#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H
#include "field.h"

class Constructor : public Field
{
public:
    Constructor(View* view);
    void createNewLevel();
    void createEmptyLevel();
    void drawFrame();
    void fillTools();
    void draw();
    virtual void processTouchMove(float x, float y);
    virtual void processTouchPress(float x, float y);
};

#endif // CONSTRUCTOR_H
