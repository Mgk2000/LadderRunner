#include "dialog.h"
#include "view.h"

Dialog::Dialog(View* _view): view(_view)
{
}

void Dialog::show()
{
    view->dialog = this;
    onShow();
}

void Dialog::close()
{
    view->dialog = 0;
}

RoundedRectangle *Dialog::roundedRectangle() const
{
    return view->roundedRect;
}

RoundedRectangle *Dialog::wideRectangle() const
{
    return view->wideRoundedRect;
}

ARectangle *Dialog::arect() const
{
    return view->rect;
}

BitmapText *Dialog::bitmapText() const
{
    return view->bitmapText;
}
