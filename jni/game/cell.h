#ifndef CELL_H
#define CELL_H
#include "drawingobject.h"
#include "texture.h"

struct Cell
{
    Cell();
    Cell(Texture::Kind _kind);
    unsigned char kind;
    void setKind(Texture::Kind _kind);
    bool free() const;
};
class CellDraw : public DrawingObject
{
public:
    CellDraw(View* view);
    void draw(Cell* cell, float _x, float _y, float _scale);
    void draw(Texture::Kind kind, float _x, float _y, float _scale);
    void initGL();
    virtual bool moveable() const {return false;}
};

#endif // CELL_H
