﻿#ifndef PLAY_H
#define PLAY_H
#include "field.h"
#include "ladder.h"
#include "undo.h"
class Bomb;
class Explosion;
class Block;
class Lift;
class Grenade;

class Play : public Field
{
public:
    Play(View* _view);
    void drawFrame();
    /*virtual void openLevel(int l);*/
    void openLevel(int l, const char* buf);

    virtual void processTouchMove(float x, float y) {}
    virtual void processTouchPress(float x, float y) ;
    virtual void processTouchRelease(float x, float y);
    bool hasSurface(int x, int y) const;
    bool isBrick(int x, int y, bool exact = false) const;
    bool isBlock(int x, int y) const;
    bool isHanging(int x, int y) const;

    bool isCorner(int x, int y) const;
    bool isLeftCorner(int x, int y) const;
    bool isRightCorner(int x, int y) const;
    bool levelRunning;
    void draw();
    void drawMoveables();
    bool pressedRightMove(float x, float y) const;
    bool pressedLeftMove(float x, float y) const;
    bool pressedUpMove(float x, float y) const;
    void moveStep();
    long long prevTime;
    void adjustScreenPosition();
    virtual bool toolEnabled(Texture::Kind kind) const;
    void drawToolbar();
    void fillTools();
    void setToolButtonsCoords();
    float cellDistance(int x1, int y1, int x2, int y2) const;
    bool canLadder(int x1, int y1, int x2, int y2) const;
    void showLadderHints();
    void hideLadderHints();
    int nRunnerKeys;
    int nRunnerGrenades;
    int grenadePutX, grenadePutY;
    void openDoor();
    bool levelDone;
    void doLevelDone();
    void drawLevelDone();
    void  drawYouDead();
    std::list <Bomb*> bombs;
    float bombBarLeft, bombBarBottom, bombBarWidth;
    float liftBarLeft, liftBarBottom, liftBarWidth;
    float grenadeBarLeft, grenadeBarBottom, grenadeBarWidth;
    virtual bool toolbarPressed(float x, float y) const;
    void processToolbarPress(float x, float y);
//    float explosionRadius, explosionRadius2;
    void doExplosion(Bomb* ebomb, std::list<Bomb*>* explosedBombs);
    std::list <Explosion*> explosions;
    void clearLevel();
    std::list<Block*> blocks;
    bool canMoveTo(int x, int y) const;
    Block* blockOfXY(int x, int y) const;
    Lift* liftOfXY(int x, int y) const;
    Ladder ladder;
    Undo undo;
    void clearBombs();
    std::list<Lift*> lifts;
    void showMainDialog();
    bool pressNearLeft, pressNearRight;
    bool firstStep;
    void fireBombBlock(Block* block);
    void extractLift(Block* block);
    void showCurrLevel() const;
    long long showCurrLevelEndTime;

};

#endif // PLAY_H
