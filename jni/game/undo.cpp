#include "undo.h"
#include "play.h"
#include "cell.h"
#include "runner.h"
#include "bomb.h"
#include "block.h"
#include "logmsg.h"
#include "growingcell.h"
#include "lift.h"

State::State() : cells(0)
{

}

void State::clear()
{
    if (!cells)
        return;
    delete [] cells;
//    for (std::list<Bomb*> ::iterator bit = bombs.begin(); bit != bombs.end(); bit++)
//        delete *bit;
//    bombs.clear();
    blocks.clear();
}


Undo::Undo() :  size(4)
{
    states = new State*[size];
    for (int i =0; i< size; i++)
        states[i] = new State;
}

Undo::~Undo()
{
}

void Undo::init(Play *_field)
{

    clear();
    field = _field;
    for (int i =0; i< size; i++)
    {
        State& st = *(states[i]);
        st.cells = new unsigned char[field->nrows * field->ncols];
        st.blocks.clear();
        for (int i=0; i< field->blocks.size(); i++)
        {
            Coords c;
            st.blocks.push_back(c);
        }
    }
    ind =0;
    len = 0;
}

void Undo::clear()
{
    for (int i = 0; i< size; i++)
    {
        states[i]->clear();
    }
}

void Undo::clearField()
{
}

void Undo::save()
{
    if (this->len < size)
        len++;
    ind = (ind+1) % size;
//    LOGD("Save ind=%d", ind);
    State& st = *states[ind];
//    st.clear();
    st.runner = Coords(field->runner->x, field->runner->y);
    st.nRunnerBombs = field->nRunnerBombs;
    st.nRunnerKeys = field->nRunnerKeys;
    st.armored = field->runner->armored;
    for (int  i = 0; i< field->nrows; i++)
        for (int j =0; j< field->ncols; j++)
            st.cells[j+i*field->ncols] = field->cells[j+i*field->ncols]->kind();
//    for (std::list<Bomb*> ::iterator bit = field->bombs.begin(); bit != field->bombs.end(); bit++)
//        st.bombs.push_back(new Bomb(**bit));
    std::list<Coords>::iterator cit = st.blocks.begin();
    for (std::list<Block*> ::iterator bit = field->blocks.begin(); bit != field->blocks.end(); bit++, cit++)
    {
        Block* block = *bit;
        Coords &c = *cit;
        c.x = block->x;
        c.y = block->y;
    }
    st.liftY.clear();
    for (std::list<Lift*>::iterator lit = field->lifts.begin(); lit!= field->lifts.end(); lit++)
    {
        Lift* lift = *lit;
        st.liftY.push_back(lift->y);
    }
}

void Undo::restore()
{
    if (len<=0)
        return;
    field->runner->revive();
    len--;
//    LOGD("Restore ind=%d", ind);
    State& st = *states[ind];
//    clearField();
    for (int i =0; i< field->nrows; i++)
        for (int j = 0; j< field->ncols; j++)
        {
            int ind = j+ i* field->ncols;
            field->cells[ind]->setKind((Texture::Kind)st.cells[ind]);
            if (field->cells[ind]->growing())
            {
                GrowingCell * gcell = (GrowingCell *)field->cells[ind];
                //unsigned char k = st.cells[ind];
                unsigned char k = gcell->_kind;
                delete gcell;
                field->cells[ind] = new Cell((Texture::Kind)k);
            }
        }
    std::list<Coords>::iterator cit = st.blocks.begin();
    for (std::list<Block*> ::iterator bit = field->blocks.begin(); bit != field->blocks.end(); bit++, cit++)
    {
        Block * block = *bit;
        Coords c = *cit;
        block->setX(c.x);
        block->setY(c.y);
    }
    field->runner->armored = st.armored;
    field->clearBombs();
    field->runner->setX(st.runner.x);
    field->runner->setY(st.runner.y);
    field->runner->setVX(0);
    field->runner->setVY(0);
    field->runner->climbing = false;

    field->nRunnerBombs = st.nRunnerBombs;
    field->nRunnerKeys = st.nRunnerKeys;
    ind = (ind+size-1) % size;
    std::list<int>::iterator it = st.liftY.begin();
    for (std::list<Lift*>::iterator lit = field->lifts.begin(); lit!= field->lifts.end(); lit++, it++)
    {
        Lift* lift = *lit;
        lift->y = *it;
    }
}


