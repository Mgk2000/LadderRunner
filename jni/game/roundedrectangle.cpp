#include "roundedrectangle.h"
#include "points.h"
#include <math.h>
RoundedRectangle::RoundedRectangle(View* view, float w, float h) : ARectangle (view),
  _width(w), _height(h)
{
    init();
}

void RoundedRectangle::init()
{
    _howDrawTriangles = GL_TRIANGLE_FAN;
    float r;
    if ( _width > _height )
        r = _height / 4;
    else r = _width /4;
    float w2 = _width /2;
    float h2 = _height /2;
    int nr = 7;
    nvertices = nr * 4;
    vertices = new Point[nvertices];
    Point c (-w2 + r, h2-r);
    float fi =0;
    float dfi = -  M_PI / 2.0 /(nr-1);
    for (int i =0; i< nvertices; i++)
    {
        vertices[i] = Point( c.x + r* sin(fi), c.y + r * cos(fi));
        if (i == nr-1)
            c = Point(-w2+r, -h2+r);
        else if (i == nr*2 -1)
            c = Point(w2-r, -h2+r);
        else if (i == nr* 3 -1)
            c= Point (w2-r, h2 -r);
        else
            fi = fi + dfi;
    }
    //vertices[nvertices-1] = vertices[0];
    //showVertices();
    initGL();
}

void RoundedRectangle::initGL()
{
    Point* vertices4 = new Point [nvertices+2];
    vertices4[0] = Point(0,0);
    for (int i=0; i< nvertices; i++)
    {
        vertices4[i+1] = vertices[i];
    }
    vertices4[nvertices+1] = vertices4[1];

    indices = new GLushort [nvertices+ 2];
    nindices = nvertices+2;
    for (int i=0; i< nindices; i++)
        indices[i] = i;
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nindices * sizeof(Point), vertices4, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices * sizeof(GLushort), indices, GL_STATIC_DRAW);

    // Transfer index data to VBO 2
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point), vertices, GL_STATIC_DRAW);
    delete[] vertices4;
    delete[] indices;
}

void RoundedRectangle::setParams(float x1, float y1, float x2, float y2, const Point4D &color)
{
    _scaleX = (x2-x1) /_width;
    _scaleY = (y2-y1) / _height;
    _color = color;
    x = (x1+x2)  /2.0;
    y = (y1+y2)  /2.0;
}
