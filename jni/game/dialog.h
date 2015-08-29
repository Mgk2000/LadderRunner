#ifndef DIALOG_H
#define DIALOG_H
class View;
class BitmapText;
class RoundedRectangle;
class ARectangle;
class Dialog
{
public:
    Dialog(View* _view);
    View* view;
    virtual void draw(){}
    virtual void show();
    virtual void onShow(){}
    virtual void close();
    virtual void processTouchPress(float x, float y) {}
    BitmapText* bitmapText() const;
    RoundedRectangle* roundedRectangle()const;
    RoundedRectangle* wideRectangle()const;
    ARectangle* arect() const;
};

#endif // DIALOG_H
