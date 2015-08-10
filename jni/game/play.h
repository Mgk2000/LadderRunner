#ifndef PLAY_H
#define PLAY_H
#include "field.h"
class Bomb;
class Play : public Field
{
public:
    Play(View* _view);
    void drawFrame();
    virtual void openLevel(int l);
    void openLevel(int l, const char* buf);

    virtual void processTouchMove(float x, float y) {}
    virtual void processTouchPress(float x, float y) ;
    virtual void processTouchRelease(float x, float y);
    bool hasSurface(int x, int y) const;
    bool isBrick(int x, int y) const;
    bool isCorner(int x, int y) const;
    bool isLeftCorner(int x, int y) const;
    bool isRightCorner(int x, int y) const;
    bool leftFree(int x, int y) const;
    bool rightFree(int x, int y) const;
    bool levelRunning;
    void draw();
    void drawMoveables();
    bool pressedRightMove(float x, float y) const;
    bool pressedLeftMove(float x, float y) const;
    bool pressedUpMove(float x, float y) const;
    bool canMoveLeft(float x, float y) const;
    bool canMoveRight(float x, float y) const;
    void moveStep();
    long long prevTime;
    void adjustScreenPosition();
    void drawToolbar();
    void fillTools();
    void setToolButtonsCoords();
    float cellDistance(int x1, int y1, int x2, int y2) const;
    bool canLadder(int x1, int y1, int x2, int y2) const;
    void showLadderHints();
    void hideLadderHints();
    bool runnerCanMove();
    int nRunnerKeys;
    void openDoor();
    bool levelDone;
    void doLevelDone();
    void drawLevelDone();
    std::list <Bomb*> bombs;
    float bombBarLeft, bombBarBottom, bombBarWidth;
    float explosionRadius, explosionRadius2;
    void doExplosion(Bomb* bomb);
 };

#endif // PLAY_H
