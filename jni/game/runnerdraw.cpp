#include "runnerdraw.h"
#include "view.h"
#include "texture.h"
#include "runner.h"
/*
101, 49 246
228,53   246
366,55   246
520,55  246
675,55  246

130, 295  485
265, 294
378, 290
485, 289
586, 292
702, 295

113, 539  730
268, 539              209-335
438, 539
573, 539
706, 534

 */

static float rCoords[17][2] = {
    {101, 246},
    {228,246},
    {366,246},
    {520,246},
    {675,246},

    {130, 485},
    {265, 485},
    {378, 485},
    {485, 485},
    {586, 485},
    {702, 485},

    {113, 729},
    {268, 729}, //              209-335
    {438, 729}, //
    {573, 729},
    {706, 729},
    {886, 738} //dead
};
static float rw = 128;
static float rh = 200;
static float fw =  0.5 * rw / 1024.0;
static float fh =  0.5 * rh /1024.0;
float hh = 1.0;
float ww = hh * rw /rh;

RunnerDraw::RunnerDraw(View* _view) : DrawingObject (_view,1,0), runner(0)
{
    texture = _view->textures[Texture::WALKING_RUNNER];
    initGL();
}

void RunnerDraw::draw(float _x, float _y, float _scale)
{
    x = _x;
    y = _y;
    this->setScale(_scale);
    if (!runner)
        offset = 0;
    else
    {
//        if (runner->alive)
//        {
//            offset = (void*) (runner->phase() * 24*4);
//        }
//        else
//        {
//            offset =  (void*) (32 * 24 * 4);
//        }

        if (runner->alive)
        {
            offset = (void*) (runner->phase() * 24*4);
            _colorMult = Point4D(1,1,1);
        }
        else
        {
            offset =  0;
            _colorMult = Point4D(0,0.5,0.5);
        }

    }
    drawTexture();

}

void RunnerDraw::initGL()
{
    Point4D* vertices4 = new Point4D[6*(16 + 16 + 1)];
    for (int i =0; i<16; i++)
        getVert(i, &vertices4[6*i], false);
    for (int i =0; i<16; i++)
        getVert(i, &vertices4[6*i + 96], true);
    getVert(16, &vertices4[6*32], false);
    glBindBuffer(GL_ARRAY_BUFFER, vboIds[0]);
    glBufferData(GL_ARRAY_BUFFER, 6*33 * sizeof(Point4D), vertices4, GL_STATIC_DRAW);
    delete[] vertices4;
}

void RunnerDraw::getVert(int icoord, Point4D *vert, bool mirror)
{
    float x = (rCoords[icoord][0])/1024.0;
    float y = (1024 - rCoords[icoord][1])/1024.0;
    float cx = x ;//+  fw;
    float cy = y +  fh;
    float k = mirror ?  -1.0 : 1.0;
    vert[0] = Point4D(0, 0, cx, cy);
//    vertices4[0] = Point4D (0 , 0, 0.5 , 0.5);
//    vertices4[1] = Point4D(-r, r, 0,1);
//    vertices4[2] = Point4D(-r, -r, 0,0.003);
//    vertices4[3] = Point4D(r, -r, 1,0.003);
//    vertices4[4] = Point4D(r, r, 1,1);

//    vertices4[5] = vertices4[1];

    vert[1] = Point4D(-ww*k, hh, cx- fw , cy+fh);
    vert[2] = Point4D(-ww*k, -hh, cx- fw , cy -fh);
    vert[3] = Point4D(ww*k, -hh, cx+ fw , cy -fh);
    vert[4] = Point4D(ww*k,  hh, cx+ fw , cy +fh);
    vert[5] = vert[1];
}
