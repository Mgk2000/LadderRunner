#include "maindialog.h"
#include "play.h"
#include "view.h"
#include "roundedrectangle.h"
#include "rectangle.h"
#include "bitmaptext.h"

MainDialog::MainDialog(View* _view): Dialog(_view)
{
    init();
}

void MainDialog::init()
{
    currLevelX = 0;
    currLevelY = 0.9;
    continueButton = Rect (-0.3, 0.5, 0.3, 0.7);
    selectLevelBottom = continueButton.bottom - 0.6;

    float dxlevel = 0.3;
    levelRect = Rect (-0.2, -0.5, 0.2, -0.3);
    levelMinusButton = Rect(-0.6, levelRect.bottom,
            -0.4, levelRect.top);
    levelPlusButton = levelMinusButton;
    levelPlusButton.shift(0.9, 0);
    okButton = Rect (-0.2, -0.7, 0.2, -0.5);
    exitButton = Rect (-0.2, -1.0, 0.2, -0.8);
}

void MainDialog::draw()
{
    arect()->draw(-1.6667,-1, 1.6667, 1, COLOR_BROWN);
    int level = view->play->level;
    char buf[32];
    sprintf(buf, "Level %d", level);
    bitmapText()->drawCenter(0, currLevelY,0.06,COLOR_WHITE,buf);
    float continueBottom = continueButton.bottom+0.08;
    wideRectangle()->drawFramed(continueButton,
         1.0, Point4D(0.0, 0.0, 0.0), Point4D(1.0, 1.0, 0.0));

    bitmapText()->drawCenter(0, continueBottom,0.06, COLOR_WHITE, "Continue");

    bitmapText()->drawCenter(0, selectLevelBottom,0.06, COLOR_YELLOW, "or select level:");
    //roundedRectangle()->drawFramed(levelRect,0.02, COLOR_BLUE, COLOR_BLACK);
    sprintf(buf,"%d", currLevel);
 //   sprintf(buf,"%f", view->aspect);
    bitmapText()->drawCenter(0,
                       levelMinusButton.bottom+0.1, 0.1, COLOR_YELLOW, buf);

    roundedRectangle()->drawFramed(levelMinusButton,0.02, COLOR_BLUE, COLOR_BLACK);
    bitmapText()->draw(levelMinusButton.left + 0.2,
                       levelMinusButton.bottom+0.1, 0.15, COLOR_YELLOW, "-");

    roundedRectangle()->drawFramed(levelPlusButton,0.02, COLOR_BLUE, COLOR_BLACK);
    bitmapText()->draw(levelPlusButton.left + 0.165,
                       levelPlusButton.bottom+0.07, 0.15, COLOR_YELLOW, "+");

    roundedRectangle()->drawFramed(okButton,0.02, COLOR_BLUE, COLOR_BLACK);
    bitmapText()->draw(okButton.left + 0.165,
                       okButton.bottom+0.07, 0.06, COLOR_YELLOW, "OK");

    roundedRectangle()->drawFramed(exitButton,0.02, COLOR_BLUE, COLOR_BLACK);
    bitmapText()->draw(exitButton.left + 0.165,
                       exitButton.bottom+0.07, 0.06, COLOR_YELLOW, "Exit");
}

void MainDialog::onShow()
{
    this->playLevel = view->play->level;
    currLevel = playLevel;
    maxLevel = view->maxLevel;
}

Play *MainDialog::field() const
{
    return view->play;
}

void MainDialog::processTouchPress(float fx, float fy)
{
    if (continueButton.inside(fx, fy))
        view->closeDialog();
    else if (levelMinusButton.inside(fx, fy) && currLevel>0)
        currLevel --;

    else if (levelPlusButton.inside(fx, fy) && currLevel<maxLevel)
        currLevel ++;
    else if (okButton.inside(fx, fy))
    {
        close();
        view->selectLevel(currLevel);
    }
    else if (exitButton.inside(fx, fy))
        view->selectExitGame();
}
