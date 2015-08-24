#ifndef GROWINGCELL_H
#define GROWINGCELL_H
#include "cell.h"
class Play;
struct GrowingCell : public Cell
{
public:
    GrowingCell(Play* _field);
    long long phase1Time;
    long long endTime;
    unsigned char kind() const;
    Play* field;
    virtual bool growing() const {return true;}
    float alpha() const;
    bool out() const;
    bool solid() const;

};

#endif // GROWINGCELL_H
