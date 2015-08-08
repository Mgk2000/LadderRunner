#include "view.h"
#include "constructor.h"
#include "play.h"
#include "logmsg.h"
#include "bitmaptext.h"
#include "roundedrectangle.h"
#include "mutex.h"
#include <sys/stat.h>
#include <time.h>
View::View() : play(0)
{
}

void View::setDirName(const char *dir)
{
    strcpy(dirName, dir);
}

int View::levelsCount() const
{
    char buf[256];
    for (int i = 0; i <1000000 ; i++)
    {
        char fn[16];
        sprintf (fn, "/level.%d", i);
        strcpy(buf, dirName);
        strcat(buf, fn);
        struct stat fstat;
        if (stat (buf, &fstat) !=0)
            return i;
    }
}
void View::resizeGL(int w, int h)
{
    width = w;
    height = h;
    glViewport(0, 0, w, h);
    aspect = w * 1.0 / h;
    projection1.setToIdentity();
    //ortho
    projection1.m[0][0] = 1.0 / aspect;
    projection1.m[2][2] = -1.0;

}

bool View::initializeGL()
{
    //LOGD("View::initializeGL() 1 level=%d levelDone=%d maxsound=%d ship=%d", _level, (int)_levelDone, (int) maxSound, (int) ship);
    glClearColor(0.0,0., 0.1, 1);
   // LOGD("View::initializeGL() 2");
    if (!initShaders())
        return false;
    //LOGD("View::initializeGL() 3");

    if (!play)
    {
      //  LOGD("View::initializeGL() 4");
        textureProgram = new TextureProgram();
        std::map<Texture::Kind, Texture*>::iterator it = textures.begin();
        for (; it!= textures.end(); it++)
            it->second->initGL(textureProgram->program());
        //LOGD("View::initializeGL() 4.1  _textures.size()=%d" , _textures.size());
        bitmapText = new BitmapText (this, textures[Texture::ABC]);
        bitmapText->init();
        //LOGD("View::initializeGL() 4.4");
        roundedRect = new RoundedRectangle(this, 1, 1.2);
        wideRoundedRect = new RoundedRectangle(this, 0.4, 0.15);
        //LOGD("View::initializeGL() 4.5");
//        pauseContinue = new PauseContinue(this, _textures[(int) Bonus::PAUSE], _textures[(int) Bonus::CONTINUE]);
//        pauseContinue->init();
        mutex = new Mutex;
        play = new Play(this);
        constructor = new Constructor(this);
        rect = new ARectangle (this);
        //LOGD("View::initializeGL() 4.6");

    }
    else //after pause pressing Home button)
    {
        //LOGD("View::initializeGL() 5");
        textureProgram->initGL();
        std::map<Texture::Kind, Texture*>::iterator it = textures.begin();
        for (; it!= textures.end(); it++)
            it->second->initGL(textureProgram->program());
        delete bitmapText;
        //LOGD("View::initializeGL() 8.11");

        bitmapText = new BitmapText (this, textures[Texture::ABC]);
        //LOGD("View::initializeGL() 8.12");
        bitmapText->init();
        //LOGD("View::initializeGL() 9");
        roundedRect->initGL();
        wideRoundedRect->initGL();
//        pauseContinue->initGL();
    }
    return true;
}
int View::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    processTouches();
#ifdef WIN32
    currTime = 1.0* clock() / CLOCKS_PER_SEC * 1000;
#else
    timeval tv;
    gettimeofday(&tv, 0);
    currTime = tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif

    field->drawFrame();
    return 0;
}

void View::newLevel()
{
    constructor->createNewLevel();
}

void View::openLevel(int l)
{
    field->openLevel(l);
}

void View::openLevel(int l, char *fn)
{
    field->openLevel(l, fn);
}
GLuint View::createShader(GLenum shaderType, const char *src)
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

GLuint View::createProgram(const char* vtxSrc, const char* fragSrc) {
    GLuint vtxShader = 0;
    GLuint fragShader = 0;
    GLuint program = 0;
    GLint linked = GL_FALSE;

    vtxShader = createShader(GL_VERTEX_SHADER, vtxSrc);
    if (!vtxShader)
        goto exit;

    fragShader = createShader(GL_FRAGMENT_SHADER, fragSrc);
    if (!fragShader)
        goto exit;

    program = glCreateProgram();
    if (!program) {
        goto exit;
    }
    glAttachShader(program, vtxShader);
    glAttachShader(program, fragShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {

        glDeleteProgram(program);
        program = 0;
    }

exit:
    glDeleteShader(vtxShader);
    glDeleteShader(fragShader);
    return program;
}

bool View::initShaders()
{
    const char* vertexstr =
    "#ifdef GL_ES\n"
    "// Set default precision to medium\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "#endif\n"
    "attribute vec3 aVertexPosition;\n"
    "uniform mat4 mvp_matrix;\n"
    "void main(void) {\n"
    "	gl_Position = mvp_matrix * vec4(aVertexPosition, 1.0);\n"
    "	}\n";

    const char* fragstr =
    "#ifdef GL_ES\n"
    "// Set default precision to medium\n"
    "precision mediump int;\n"
    "precision mediump float;\n"
    "#endif\n"
    "uniform vec4 color;\n"
    "void main(void) {\n"
    "	  gl_FragColor = color;\n"
    "	}\n";
    program = createProgram(vertexstr, fragstr);
    if (!program)
        return false;

    colorlocation = glGetUniformLocation(program, "color");
    matrixlocation = glGetUniformLocation(program, "mvp_matrix");
    vertexlocation = glGetAttribLocation(program, "aVertexPosition");
    return true;
}

void View::runConstructor()
{
    field = constructor;
}
void View::runPlay()
{
    field = play;
}


void View::addTexture(int w, int h, const char* data, Texture::Kind kind, bool transparentWhite)
{
    if ((int) kind ==0)
    {
        if (textures.size() != 0)
            textures.clear();
    }
    Texture* texture = new Texture (w,h, data, kind, transparentWhite);
    textures[kind] = texture;
   // LOGD("View::addTexture textures.size =%d", _textures.size());
}
void View::screenToView(int x, int y, float *fx, float *fy) const
{
    *fx = 2.0 * (x - width/2) / width * aspect;
    *fy = - 2.0 * (y - height/2) * 1.0 / height;
}

void View::onTouchEvent(int what, int x, int y)
{
    mutex->lock();
    TouchEvent te (what, x, y);
    touches.push_back(te);
    mutex->unlock();
}
void View::processTouches()
{
    mutex->lock();
    int nt = touches.size();
    for (std::list<TouchEvent>::const_iterator it2 = touches.begin();
         it2 != touches.end(); it2++)
        {
            const TouchEvent &te = *it2;
            switch(te.type)
            {
            case TouchPointPressed:
                processTouchPress(te.x, te.y);
                break;
            case TouchPointMoved:
                processTouchMove(te.x, te.y);
                break;
            case TouchPointReleased:
                processTouchRelease(te.x, te.y);
                break;
            default: break;
            }
        }
    touches.clear();
    mutex->unlock();
}
void View::processTouchMove(int x, int y)
{
    float fx, fy;
    screenToView(x,y, &fx, &fy);
    field->processTouchMove(fx, fy);
}
void View::processTouchPress(int x, int y)
{
    float fx,fy;
    screenToView(x,y, &fx, &fy);
    field->processTouchPress(fx, fy);
}
void View::processTouchRelease(int x, int y)
{
    float fx,fy;
    screenToView(x,y, &fx, &fy);
    field->processTouchRelease(fx, fy);
}
