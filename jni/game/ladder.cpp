#include "ladder.h"
#include "play.h"
#include "math_helper.h"
Ladder::Ladder(View* _view, Play* _field) : DrawingObject (_view, 1, 0), field (_field)
{
    nvertices = 6;
    vertices = new Point[nvertices];
    vertices[0] = Point(-0.1, -1.0);
    vertices[1] = Point(-0.1, 1.0);
    vertices[2] = Point(0.1, -1.0);
    vertices[3] = Point(0.1, 1.0);
    vertices[4] = Point(-0.1, 0.0);
    vertices[5] = Point(0.1, 0.0);
    initGL();
}

void Ladder::setCoords(float _x1, float _y1, float _x2, float _y2, float __angle)
{
    x1 = _x1;
    y1 = _y1 - 0.5;
    x2 = _x2;
    y2 = _y2 - 0.5;
    if ( x2 > x1)
        x2 = x2 - 0.5;
    else if (x1 > x2)
        x2 = x2 + 0.5;
    angle = atan2(x2-x1, y2-y1);
   // angle = __angle;
//    float sc = field->scale;
    float len = dist(x1, y1, x2, y2);
    nl = len * 6;
}

void Ladder::draw()
{
    float sc = field->scale;
    float r = 2.0/12;
    float dx = r * sin(angle);
    float dy = r * cos(angle);
    this->setScale(1.0/12 * sc);
    for (int i =0; i <nl; i++)
    {
        float xx = 1.0 * x1 + i * dx;
        float yy = 1.0 * y1 + i * dy;
        field->fieldToScreen(xx, yy, &x, &y);
        drawLines(GL_LINES, vboIds[0], nvertices, COLOR_BLUE,1.0, -angle );
    }
}

void Ladder::initGL()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point), vertices, GL_STATIC_DRAW);

}
