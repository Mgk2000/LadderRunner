#include "drawingobject.h"
#include "view.h"
#include "math.h"
#include "math_helper.h"
#include "logmsg.h"
#include "texture.h"
#include "mat4.h"

DrawingObject::DrawingObject(View* _view, int _nbos, Texture* __texture): nvbos (_nbos), vertices(0), indices(0), nvertices(0), nindices(0), view(_view),
      _scale(1.0), texture(__texture),
    _colorMult(1.0,1.0,1.0), _scaleX(1.0), _scaleY(1.0),
    _howDrawTriangles (GL_TRIANGLES), offset(0)
{
    if (nvbos)
    {
        vboIds = new uint[nvbos];
        glGenBuffers(nvbos, vboIds);
    }
    _startTime = currTime();

}

DrawingObject::DrawingObject(View *_view, int _nbos, float _x, float _y, float _speed, float _angle):
    nvbos (_nbos), vertices(0), indices(0), nvertices(0), nindices(0), view(_view),
    x(_x), y(_y) , _scale(1.0), texture(0),
    _colorMult(1.0,1.0,1.0),
    _scaleX(1.0), _scaleY(1.0),
   _howDrawTriangles (GL_TRIANGLES), offset(0)

{
    if (nvbos)
    {
        vboIds = new uint[nvbos];
        glGenBuffers(nvbos, vboIds);
    }

    _startTime = currTime();
}

DrawingObject::DrawingObject(View *_view, int _nbos, float _x, float _y, float _speed,
    float _angle, Texture *__texture):
nvbos (_nbos), vertices(0), indices(0), nvertices(0), nindices(0), view(_view),
x(_x), y(_y) ,
  _scale(1.0), texture(__texture), _colorMult(1.0,1.0,1.0)
, _scaleX(1.0), _scaleY(1.0),
  _howDrawTriangles (GL_TRIANGLES), offset(0)

{
    if (nvbos)
    {
        vboIds = new uint[nvbos];
        glGenBuffers(nvbos, vboIds);
    }
    _startTime = currTime();
}

DrawingObject::~DrawingObject()
{
    if (nvbos)
    {
        glDeleteBuffers(nvbos, vboIds);
        delete[] vboIds;
    }
    if (vertices)
        delete[] vertices;
    if (indices)
        delete[] indices;
}

void DrawingObject::init()
{
}

void DrawingObject::draw()
{
}

void DrawingObject::moveStep(float delta)
{
}

void DrawingObject::getCurrentCoords(Point *_vertices, int *_nvertices) const
{
    *_nvertices = nvertices;
    for (int i=0; i< nvertices; i++)
    {
        _vertices[i].x = vertices[i].x+x;
        _vertices[i].y = vertices[i].y+y;
    }
}

void DrawingObject::fill_vbos()
{
    // Transfer vertex data to VBO 0
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, nvertices * sizeof(Point), vertices, GL_STATIC_DRAW);

    // Transfer index data to VBO 1
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, nindices * sizeof(GLushort), indices, GL_STATIC_DRAW);
}

void DrawingObject::drawTriangles (uint vbo)
{
    Mat4 _matrix1;
    _matrix1.translate(x, y, 0);
    _matrix1 = view->projection1 * _matrix1;
    glUseProgram(view->getProgram());
    _matrix1.scale(_scaleX, _scaleY , 1.0);
    glUniformMatrix4fv(view->matrixlocation, 1, false, (const GLfloat*) &_matrix1);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(view->vertexlocation);
    glVertexAttribPointer(view->vertexlocation, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void *) 0);
    Point4D col = color();
    glUniform4fv(view->colorlocation ,1 , (GLfloat*) &col );
    glDrawElements(_howDrawTriangles, nindices , GL_UNSIGNED_SHORT, 0);
    glDisableVertexAttribArray(view->vertexlocation);
}

void DrawingObject::drawLines(int how, uint vbo, int npoints, const Point4D& _color, float _width, float angle)
{
//	int err;
    Mat4 _matrix1;
    _matrix1.translate(x, y, 0);
    _matrix1 = view->projection1 * _matrix1;
    //	glUseProgram(view->flyingprogram().programId());
        glUseProgram(view->program);
    if (_scale<0.99 ||_scale>1.1)
    {
        Mat4 scaleMatrix(_scale);
        _matrix1=_matrix1 * scaleMatrix;
    }
    _matrix1.scale(_scaleX, _scaleY , 1.0);
    glUniformMatrix4fv(view->matrixlocation, 1, false, (const GLfloat*) &_matrix1);
//	err = glGetError();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	err = glGetError();
    glEnableVertexAttribArray(view->vertexlocation);
//	err = glGetError();
    glVertexAttribPointer(view->vertexlocation, 3, GL_FLOAT, GL_FALSE, sizeof(Point), (const void *) 0);
//	err = glGetError();
    Point4D col = _color;
    glUniform4fv(view->colorlocation ,1 , (GLfloat*) &col );
//	err = glGetError();
    glLineWidth(_width);
//	err = glGetError();
    glDrawArrays(how, 0, npoints);
//	err = glGetError();
//	LOGD("err=%d", err);
    glDisableVertexAttribArray(view->vertexlocation);
//	err = glGetError();
}

long long DrawingObject::currTime() const
{
    return view->getCurrTime();
}

BitmapText *DrawingObject::bitmapText() const
{
    return view->getBitmapText();
}

void DrawingObject::drawTexture(float angle)
{
    Mat4 _matrix1;
    _matrix1.translate(x, y, 0);
    _matrix1 = view->projection1 * _matrix1;
    if (_scale<0.99 ||_scale>1.1)
    {
        Mat4 scaleMatrix(_scale);
        _matrix1=_matrix1 * scaleMatrix;
    }
    int err;
    glUseProgram(texture->program());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glActiveTexture(GL_TEXTURE0);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glBindTexture(GL_TEXTURE_2D, texture->textureId());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//    glBlendFunc (GL_SRC_COLOR, GL_DST_COLOR);
//    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glUniformMatrix4fv(texture->matrixLocation(), 1, false,
                       (const GLfloat*) &_matrix1);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
   // _colorMult.alpha = 0.5;
    glUniform4fv(texture->colorMultLocation(), 1,
                       (const GLfloat*) &_colorMult);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIds[1]);
//    err = glGetError();
//    if (err)
//        LOGD("err=%d", err);
    int texturelocation = texture->textureLocation();
    glUniform1i(texturelocation, 0);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);

//    quintptr offset = 0;
    void* _offset = offset;
    int vertexLocation = texture->posLocation();
    glEnableVertexAttribArray(vertexLocation);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);

    glVertexAttribPointer(vertexLocation, 2, GL_FLOAT, GL_FALSE,
                          sizeof(Point4D), (const void *)_offset);
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    _offset += 8;

    glEnableVertexAttribArray(texture->texCoordLocation());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    glVertexAttribPointer(texture->texCoordLocation(), 2, GL_FLOAT,
                          GL_FALSE, sizeof(Point4D), (const void *)_offset);
    err = glGetError();
//    glDrawElements(how, nindices, GL_UNSIGNED_SHORT, 0);
    glDrawArrays(GL_TRIANGLE_FAN, 0, nTextureVertices());
    err = glGetError();
    if (err)
        LOGD("err=%d", err);
    err = glGetError();
    glDisableVertexAttribArray(texture->texCoordLocation());
    glDisableVertexAttribArray(vertexLocation);

}
