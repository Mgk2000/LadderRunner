#include "texture.h"
#include "logmsg.h"
#include <stdio.h>
#include <cstring>
static const char vertexShader[] =
        "precision mediump float;                            \n"
        "precision mediump int;                            \n"
        "//attribute vec4 a_position;   \n"
        "attribute vec2 a_position;   \n"
        "attribute vec2 a_texCoord;   \n"
        "uniform mat4 mvp_matrix;     \n"
        "varying vec2 v_texCoord;     \n"
        "void main()                  \n"
        "{                            \n"
        "//   gl_Position = a_position; \n"
        "	gl_Position = mvp_matrix * vec4(a_position,0.0, 1.0);\n"
        "   v_texCoord = a_texCoord;  \n"
        "}                            \n";

static const char fragmentShader[] =
        "precision mediump float;                            \n"
        "precision mediump int;                            \n"

        "varying vec2 v_texCoord;                            \n"
        "uniform sampler2D s_texture;                        \n"
        "uniform vec4 s_colormult;                           \n"
        "void main()                                         \n"
        "{                                                   \n"
        "  vec4 col = texture2D( s_texture, v_texCoord );\n"
        "  vec4 col1 = vec4(2.0, 0.5, 0.5, 0.3) ;                 \n"
        "  for(int i=0; i<4; i++)                           \n"
        "       col[i] = (col[i]*s_colormult[i]);             \n"
        "  //  col[0] = (col[0]+col1[0])/2.0;                \n"
        "  //col[1] = (col[1]+col1[1])/2.0;                \n"
        "  //col[2] = (col[2]+col1[2])/2.0;                \n"
        "  gl_FragColor = col;                              \n"
        "  //gl_FragColor = texture2D( s_texture, v_texCoord );\n"
        "}                                                   \n";
Texture::Texture(int w, int h, const char* data, Kind __kind, bool transparentWhite)
    : _program(0), _kind(__kind),
    bmpdata((char*) data)
{
    loadPicture(w, h, data, transparentWhite);
}

Texture::~Texture()
{
//    delete[] bmpdata;
}

GLubyte *Texture::getFragment(GLubyte *src, GLubyte* dst,
    int x, int y, int width, int height) const
{
    for (int i=0; i < height; i++)
    {
        GLubyte* sptr = &src[(y+i) * _picWidth *3 + x];
        GLubyte* dptr =&dst[i*width];
        memcpy(dptr, sptr, width);
    }
}
bool Texture::initGL(int __program)
{
    _program = __program;
    if (!_program)
    {
        LOGD("Could not create program.");
        return false;
    }
    _posLocation = glGetAttribLocation(_program, "a_position");
    _texCoordLocation = glGetAttribLocation(_program, "a_texCoord");
    _textureLocation = glGetUniformLocation(_program, "s_texture");
    _matrixLocation  = glGetUniformLocation(_program, "mvp_matrix");
    _colorMultLocation  = glGetUniformLocation(_program, "s_colormult");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glGenTextures(1, &_textureId);
    createTexture();
    //delete[] bmpdata;
    return true;
}


void Texture::loadPicture(int picWidth, int picHeight, const char* data, bool transparentWhite)
{
    unsigned char trb = transparentWhite ? 255 : 0;
    // Read data from file into texture
    GLubyte* inData;
    _picWidth = picWidth;
    _picHeight = picHeight;
    int  size = _picWidth* _picHeight*4;
    char * ccc = new char[size];
    pcData = (GLubyte*) ccc;
    //pcData = new GLubyte[size];
    inData = (GLubyte*)&data[0];
    for (int i=0; i< _picWidth * _picHeight; i++)
    {
        unsigned char r = inData[i*3+2];
        unsigned char g = inData[i*3+1];
        unsigned char b = inData[i*3];
        unsigned char a = 255;
        if (r==trb && g==trb && b==trb)
            a = 0;
        pcData[i*4] = r;
        pcData[i*4+1] = g;
        pcData[i*4+2] = b;
        pcData[i*4+3] = a;
    }
}
void Texture::createTexture()
{
//        int err;
        glBindTexture(GL_TEXTURE_2D, _textureId);
//        err = glGetError();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _picWidth, _picHeight,
           0, GL_RGBA, GL_UNSIGNED_BYTE, pcData);
//        err = glGetError();
        glEnable(GL_TEXTURE_2D) ;

        glGenerateMipmap(GL_TEXTURE_2D);
//        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        err = glGetError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        err = glGetError();
}


TextureProgram::TextureProgram()
{
    createProgram();
}

void TextureProgram::initGL()
{
    createProgram();
}

void TextureProgram::createProgram()
{
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vertexShader);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragmentShader);
    if (!fragShader)
        goto exit;

    _program = glCreateProgram();
    if (!_program) {
        goto exit;
    }
    glAttachShader(_program, vtxShader);
    glAttachShader(_program, fragShader);

    glLinkProgram(_program);
    glGetProgramiv(_program, GL_LINK_STATUS, &linked);
    if (!linked)
    {

        glDeleteProgram(_program);
        _program = 0;
    }

exit:
    ;
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);

}

GLuint TextureProgram::createShader(GLenum shaderType, const char *src)
{
    GLuint shader = glCreateShader(shaderType);
    if (!shader) {
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);

    GLint compiled = GL_FALSE;
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        glDeleteShader(shader);
        return 0;
    }
    return shader;

}
