#ifndef CELL_H
#define CELL_H
#include "drawingobject.h"
#include "texture.h"

struct Cell
{
    Cell();
    Cell(Texture::Kind _kind);
    virtual unsigned char kind() const {return _kind;}
    unsigned char saveKind;
    void setKind(Texture::Kind _kind, bool save = false);
    void restoreKind();
    bool free() const;
    bool breakable() const;
    virtual bool growing() const {return false;}
    unsigned char _kind;
protected:
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
