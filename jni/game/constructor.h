#ifndef CONSTRUCTOR_H
#define CONSTRUCTOR_H
#include "field.h"

class Constructor : public Field
{
public:
    Constructor(View* view);
    void createNewLevel(int l);
    void createEmptyLevel();
    void drawFrame();
    void fillTools();
    void draw();
    virtual void processTouchMove(float x, float y);
    virtual void processTouchPress(float x, float y);
    void resizeField(int newNCols, int newNRows);
    virtual void drawToolbar();
    void saveLevel();

};

#endif // CONSTRUCTOR_H
