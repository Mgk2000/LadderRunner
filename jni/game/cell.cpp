#include "cell.h"
#include "view.h"

Cell::Cell() : kind((unsigned char)Texture::EMPTY) , saveKind(Texture::EMPTY)
{

}

Cell::Cell(Texture::Kind _kind) :
     kind((unsigned char)_kind), saveKind(Texture::EMPTY)
{
}

void Cell::setKind(Texture::Kind _kind, bool save)
{
    if (save)
        saveKind = kind;
    kind = (unsigned char) _kind;
}

void Cell::restoreKind()
{
    kind = saveKind;
}

bool Cell::free() const
{
    switch ((Texture::Kind) this->kind)
    {
    case Texture::EMPTY:
    case Texture::GOLDEN_KEY:
    case Texture::DOOR:
    case Texture::BOMB:
    case Texture::PLACE:
    case Texture::OPEN_DOOR:
        return true;
    default:
        return false;
    }
}

bool Cell::breakable() const
{
    switch ((Texture::Kind) this->kind)
    {
    case Texture::BRICK:
    case Texture::BIG_BRICK:
    case Texture::SOFT_BRICK:
    case Texture::BIG_SOFT_BRICK:
        return true;
    default:
        return false;
    }
}


CellDraw::CellDraw(View *_view) : DrawingObject(_view, 1, 0)
{
    initGL();
}

void CellDraw::draw(Cell *cell, float _x, float _y, float _scale)
{
    draw ((Texture::Kind)cell->kind, _x, _y, _scale);
}

void CellDraw::draw(Texture::Kind kind, float _x, float _y, float _scale)
{
    if (kind == Texture::EMPTY)
        return;
    texture = view->textures[kind];
    x = _x;
    y = _y;
    this->setScale(_scale);
    drawTexture();

}

void CellDraw::initGL()
{
    float r = 1.0;
    Point4D* vertices4 = new Point4D[6];
    vertices4[0] = Point4D (0 , 0, 0.5 , 0.5);
    vertices4[1] = Point4D(-r, r, 0,1);
    vertices4[2] = Point4D(-r, -r, 0,0.003);
    vertices4[3] = Point4D(r, -r, 1,0.003);
    vertices4[4] = Point4D(r, r, 1,1);
    vertices4[5] = vertices4[1];
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;
}
