#ifndef UNDO_H
#define UNDO_H
#include <list>
class Block;
class Lift;
class Bomb;
class Play;
class Runner;
struct Coords
{
    Coords(float i, float j) {x = i; y = j;}
    Coords(){x=0; y=0;}
    float x, y;
};

struct State
{
    State();
    unsigned char* cells;
    std::list<Block*> blocks;
    Coords runner;
    int nRunnerBombs, nRunnerKeys, nRunnerGrenades;
    void clear();
    bool armored;
    std::list<Lift*> lifts;
};

class Undo
{
public:
    Undo();
    ~Undo();
    int size, len, ind;
    State** states;
    void init(Play* _field);
    bool canRestore() const {return len > 0;}
    void clear();
    void clearField();
    void save();
    void copyLifts(std::list<Lift*> *dst, std::list<Lift*> *src);
    void copyBlocks(std::list<Block*> *dst, std::list<Block*> *src);
    void saveBlocks();
    void saveLifts();
    void restoreBlocks();
    void restoreLifts();
    void restore();
    Play* field;
};

#endif // UNDO_H
