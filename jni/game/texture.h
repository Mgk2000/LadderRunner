#ifndef TEXTURE_H
#define TEXTURE_H
#include <globject.h>
class TextureProgram : public GlObject
{
public:
    TextureProgram();
    void initGL();
    int program() const {return _program;}
private:
    void createProgram();
    GLuint createShader(GLenum shaderType, const char* src);
    int _program;
private:

};

class Texture : public GlObject
{
public:
    enum Kind
    {
        EMPTY,
        CROSS,
        RUNNER,
        BRICK,
        SOLID_BRICK,
        SOFT_BRICK,
        DOOR,
        GOLDEN_KEY,
        ZOOM_IN,
        ZOOM_OUT,
        MOVE,
        LADDER,
        ABC
    };
    Texture(int w, int h, const char* data, Kind kind, bool transparentWhite);
    ~Texture();
    int posLocation() const {return _posLocation;}
    int textureLocation() const {return _textureLocation;}
    int texCoordLocation() const {return _texCoordLocation;}
    int matrixLocation() const {return _matrixLocation;}
    int colorMultLocation() const {return _colorMultLocation;}
    int program() const {return _program;}
    int picWidth() const {return _picWidth;}
    int picHeight() const {return _picHeight;}
    bool initGL(int __program);
    GLuint textureId() const {return _textureId;}
    int kind() const {return _kind;}
private:
    GLubyte* getFragment( GLubyte *src, GLubyte* dst, int x, int y, int width, int height) const;
    void loadPicture(int, int, const char* data, bool transparentWhite);
    void createTexture();
    GLubyte* pcData;
    int _posLocation,
    _textureLocation,
    _texCoordLocation,
    _colorMultLocation,
    _matrixLocation;
    int _picWidth, _picHeight;
    int _program;
    GLuint _textureId;
    Kind _kind;
    char * bmpdata;
};

#endif // TEXTURE_H
