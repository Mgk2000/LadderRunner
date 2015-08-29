#ifndef UNDO_H
#define UNDO_H
#include <list>
class Block;
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
    std::list<Coords> blocks;
    Coords runner;
    int nRunnerBombs, nRunnerKeys;
    void clear();
    bool armored;
    std::list<int> liftY;
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
    void restore();
    Play* field;
};

#endif // UNDO_H
