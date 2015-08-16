#include "rectangle.h"
#include "view.h"

ARectangle::ARectangle(View* view) : DrawingObject(view, 3, 0)
{
    init();
}

void ARectangle::init()
{
    vertices = new Point[6];
    vertices[0] = Point (-1.0, 1.0);
    vertices[1] = Point (-1.0, -1.0);
    vertices[2] = Point (1.0, -1.0);
    vertices[3] = Point (1.0, 1.0);
    vertices[4] = Point (-1.0, 1.0);
    vertices[5] = Point (1.0, -1.0);

    nvertices = 4;

    indices = new GLushort [6];
    for (int i = 0; i< 6; i++)
        indices[i] = i;
    nindices = 6;
    initGL();
}

void ARectangle::initGL()
{
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(Point), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices * sizeof(GLushort), indices, GL_STATIC_DRAW);

    // Transfer index data to VBO 2
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[2]);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(Point), vertices, GL_STATIC_DRAW);
}

void ARectangle::draw(float x1, float y1, float x2, float y2, const Point4D &color)
{
    setParams(x1, y1, x2, y2, color);
//    _color.alpha = 0.3;
    drawTriangles (vboIds[1]);
}

void ARectangle::drawFrame(float x1, float y1, float x2, float y2, float lwidth,  const Point4D &color)
{
    setParams(x1, y1, x2, y2, color);
    drawLines(GL_LINE_LOOP, vboIds[2], nvertices, color , lwidth);
}

void ARectangle::drawFramed(float x1, float y1, float x2, float y2, float lwidth,
                            const Point4D& color, const Point4D &framecolor)
{
    setParams(x1, y1, x2, y2, color);
    drawTriangles (vboIds[1]);
    _scaleX = _scaleX * (x2-x1) / (x2-x1 + lwidth/view->width);
    _scaleY= _scaleY * (y2-y1) / (y2-y1 + lwidth/view->height);
    drawLines(GL_LINE_LOOP,vboIds[2], nvertices, framecolor , lwidth);
}

void ARectangle::setParams(float x1, float y1, float x2, float y2, const Point4D& color)
{
    _scaleX = (x2-x1) /2.0;
    _scaleY = (y2-y1) /2.0;
    _color = color;
    x = (x1+x2)  /2.0;
    y = (y1+y2)  /2.0;
}
