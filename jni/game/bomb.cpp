#include "bomb.h"
#include "texture.h"
#include "play.h"
#include "view.h"

Bomb::Bomb(View* _view, Play* _field, Texture* _texture) : DrawingObject (_view ,1 , _texture),
     field(_field), phase(0)
{
    this->setScale(0.05);
    initGL();
    beginTime = field->currTime();
    endTime = beginTime + 3000;
}

void Bomb::draw()
{
    offset = (void*) (phase * 24*4);
    field->fieldToScreen(ix, iy, &x, &y);
    drawTexture();
    return;
}

void Bomb::initGL()
{
    float _width = 1.0;
    Point4D* vertices4 = new Point4D[24];
    vertices4[0] = Point4D (0 , 0, 0.25 , 0.25);
    vertices4[1] = Point4D(-_width, _width, 0, 0.5);
    vertices4[2] = Point4D(-_width, -_width, 0,0);
    vertices4[3] = Point4D(_width, -_width , 0.5,0);
    vertices4[4] = Point4D(_width, _width, 0.5 ,0.5);
    vertices4[5] = vertices4[1];

    vertices4[6] = Point4D (0 , 0, 0.75 , 0.25);
    vertices4[7] = Point4D(-_width, _width, 0.5, 0.5);
    vertices4[8] = Point4D(-_width, -_width, 0.5,0);
    vertices4[9] = Point4D(_width, -_width , 1.0,0);
    vertices4[10] = Point4D(_width, _width, 1.0 ,0.5);
    vertices4[11] = vertices4[7];

    vertices4[12] = Point4D (0 , 0, 0.25 , 0.75);
    vertices4[13] = Point4D(-_width, _width, 0, 1.0);
    vertices4[14] = Point4D(-_width, -_width, 0,0.5);
    vertices4[15] = Point4D(_width, -_width , 0.5,0.5);
    vertices4[16] = Point4D(_width, _width, 0.5 ,1.0);
    vertices4[17] = vertices4[13];

    vertices4[18] = Point4D (0 , 0, 0.75 , 0.75);
    vertices4[19] = Point4D(-_width, _width, 0.5, 1.0);
    vertices4[20] = Point4D(-_width, -_width, 0.5,0.5);
    vertices4[21] = Point4D(_width, -_width , 1.0,0.5);
    vertices4[22] = Point4D(_width, _width, 1 ,1);
    vertices4[23] = vertices4[19];

    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;

}

void Bomb::moveStep(float delta)
{
    int n = (field->currTime() -lastTime);
    if (n> 100)
    {
        lastTime = field->currTime();
        phase = (phase +1) % 4;
    }
}
bool Bomb::out() const
{
    return currTime() >= endTime;
}
