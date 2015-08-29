#ifndef MAINDIALOG_H
#define MAINDIALOG_H
#include "dialog.h"
#include "points.h"
class Play;
class View;
class MainDialog : public Dialog
{
public:
    MainDialog(View* _view);
    void init();
    virtual void draw();
    void onShow();
    Play* field() const;
    float currLevelX, currLevelY;
    Rect okButton;
    Rect continueButton;
    Rect levelMinusButton;
    Rect levelPlusButton;
    Rect levelRect;
    Rect exitButton;
    float selectLevelBottom;
    void processTouchPress(float fx, float fy);
    int playLevel, currLevel, maxLevel;

};

#endif // MAINDIALOG_H
