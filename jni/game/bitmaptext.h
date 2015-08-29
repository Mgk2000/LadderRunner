#ifndef BITMAPTEXT_H
#define BITMAPTEXT_H
#include <map>
#include "drawingobject.h"
class Letter : public DrawingObject
{
protected:
    Letter(View* view, Texture* texture);
    float width;
    int index;
    void init(int nrows);
    void initGL();
    friend class BitmapText;
};
class View;
class Texture;
class BitmapText
{
public:
    BitmapText(View* view, Texture* texture);
    void init();
    void draw(float x, float y, float scale, const Point4D &color, const char *txt);
    void drawCenter(float x, float y, float scale, const Point4D &color, const char *txt);
    void initGL();
    float textWidth(const char* txt);
private:
    std::map <char, Letter*> letters;
    int cellSize;
    int nrows;
};

#endif // BITMAPTEXT_H
