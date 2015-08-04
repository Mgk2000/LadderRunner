#ifndef DRAWINGOBJECT_H
#define DRAWINGOBJECT_H

#include <globject.h>
#include "points.h"
//class Random;
class View;
class Mat4;
class Texture;
//class Text;
class BitmapText;
//#define MAXVERTICES 60
class DrawingObject : public GlObject
{
public:
    DrawingObject(View* _view, int _nbos, Texture* __texture);
    DrawingObject(View* _view, int _nbos, float _x, float _y, float _speed, float _angle);
    DrawingObject(View* _view, int _nbos, float _x, float _y, float _speed, float _angle, Texture* __texture);
    virtual ~DrawingObject();
    virtual void init();
    virtual void draw();
    virtual void moveStep(float delta);
    float X() const {return x;}
    void setX(float _x){x=_x;}
    float Y() const {return y;}
    void setY(float _y){y=_y;}
    inline void setColor(float r , float g , float b) {_color = Point4D (r,g,b,1.0);}
    inline void setColor( const Point4D&  __color) {_color = __color;}
    inline virtual const Point4D&  color() const {return _color;}
    virtual void getCurrentCoords (Point * _vertices, int* _nvertices ) const;
    inline void setScale (float sc) {_scale = sc;}
    inline virtual int cost() const {return 0;}
    inline float r() const {return _r;}
    const Point4D& colorMult() const {return _colorMult;}
    long long currTime() const;
    float scaleX () const {return _scaleX;}
    float scaleY () const {return _scaleY;}
    BitmapText* bitmapText() const;
    void setColorMult( const Point4D cm) {_colorMult = cm;}
protected:
    float x,y;
    uint* vboIds;
    int nvbos;
    Point* vertices;
    int nvertices;
    GLushort* indices;
    void showVertices();
    int nindices;
    Point4D _color;
    void fill_vbos();
    View* view;
    float _r;
    void drawTriangles(uint vbo);
    void drawLines(int how, uint vbo, int npoints, const Point4D& color, float _width, float angle = 0.0);
    float _scale, _scaleX, _scaleY;
    Texture* texture;
    void drawTexture( float angle = 0.0);
    Point4D _colorMult;
    long long _startTime;
    int _shootCount, _breakCount;
    GLenum _howDrawTriangles;
    void* offset;
    virtual int nTextureVertices() const {return 6;}

};
#endif // DRAWINGOBJECT_H
