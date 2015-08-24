#include "growingcell.h"
#include "play.h"
GrowingCell::GrowingCell(Play* _field) : Cell(), field(_field)
{
    phase1Time = field->currTime() + 3000;
    endTime = phase1Time + 2000;
}

unsigned char GrowingCell::kind() const
{
    if (field->currTime() < phase1Time)
        return (unsigned char) Texture::EMPTY;
    else
        return _kind;
}

float GrowingCell::alpha() const
{
    if (field->currTime() < phase1Time || field->currTime()>=endTime )
        return 1.0;
    else
        return 1.0 * (field->currTime() - phase1Time) / (endTime-phase1Time);
}

bool GrowingCell::out() const
{
    return field->currTime() >= endTime;
}

bool GrowingCell::solid() const
{
    return field->currTime()>= phase1Time && field->currTime()< endTime;
}
