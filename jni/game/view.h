#ifndef VIEW_H
#define VIEW_H
#include "globject.h"
#include "mat4.h"
class Play;
class Constructor;
class Field;
#include "texture.h"
class BitmapText;
class TextureProgram;
class RoundedRectangle;
class Mutex;
class ARectangle;
class Dialog;
class MainDialog;
enum TouchPointState {
    TouchPointPressed    = 0x01,
    TouchPointMoved      = 0x02,
    TouchPointReleased   = 0x08
};
struct TouchEvent
{
    TouchEvent(int _type, int _x, int _y):
        type((TouchPointState) _type), x(_x), y(_y){}
TouchPointState type;
int x, y;
};

class View : public GlObject
{
public:
    View();
    BitmapText * getBitmapText() const {return bitmapText;}
    long long getCurrTime() const {return currTime;}
    int width, height;
    int getProgram() const {return program;}
    void setDirName(const char* dir);
    const char* getDirName() const {return (const char*) dirName;}
    int levelsCount() const;
    void newLevel(int l);
    void openLevel(int l);
    void openLevel(int l, char* fn);
public:
    char dirName[256];
    Constructor* constructor;
    Play* play;
    Field* field;
    bool initializeGL();
    void resizeGL(int w, int h);
    bool initShaders();

    void runConstructor();
    void runPlay();
    std::map<Texture::Kind, Texture*> textures;
    void addTexture (int w, int h, const char* data,
                                        Texture::Kind kind, bool transparentWhite);
    BitmapText * bitmapText;
    long long  currTime;
    TextureProgram* textureProgram;
    RoundedRectangle* roundedRect, * wideRoundedRect;
    ARectangle* rect;
    Mutex* mutex;
    int matrixlocation, vertexlocation, colorlocation;
    int program;
    GLuint createProgram(const char *pVertexSource, const char *pFragmentSource);
    GLuint createShader(GLenum shaderType, const char* src);
    Mat4 projection1;
    void screenToView(int x, int y, float* fx, float * fy) const;

    std::list<TouchEvent>touches;
    void onTouchEvent(int what, int x, int y);
    void processTouches();
    void processTouchMove (int x, int y);
    void processTouchPress (int x, int y);
    float aspect;
    int drawFrame();
    void processTouchRelease(int x, int y);
    Dialog* dialog;
    MainDialog* mainDialog;
    void showMainDialog();
    int maxSound;
    void sound(int sound);
    void closeDialog();
    void setMaxLevel(int l){maxLevel = l;}
    int maxLevel;
    void selectLevel(int l);
    void selectExitGame();
};


#endif // VIEW_H
