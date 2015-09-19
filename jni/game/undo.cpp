#include "undo.h"
#include "play.h"
#include "cell.h"
#include "runner.h"
#include "bomb.h"
#include "block.h"
#include "bombblock.h"
#include "liftblock.h"
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


Undo::Undo() :  size(100)
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

    for (int i = 0; i< size; i++)
        states[i]->cells = new unsigned char [field->nrows * field->ncols];
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
    st.nRunnerGrenades = field->nRunnerGrenades;
    st.armored = field->runner->armored;
    for (int  i = 0; i< field->nrows; i++)
        for (int j =0; j< field->ncols; j++)
            st.cells[j+i*field->ncols] = field->cells[j+i*field->ncols]->_kind;
//    for (std::list<Bomb*> ::iterator bit = field->bombs.begin(); bit != field->bombs.end(); bit++)
//        st.bombs.push_back(new Bomb(**bit));
    saveBlocks();
    saveLifts();
}

void Undo::copyLifts(std::list<Lift *> *dst, std::list<Lift *> *src)
{
    for (std::list<Lift*>::iterator lit = dst->begin(); lit!= dst->end(); lit++)
        delete *lit;
    dst->clear();
    for (std::list<Lift*>::iterator lit = src->begin(); lit!= src->end(); lit++)
    {
        Lift* lift = new Lift(**lit);
        dst->push_back(lift);
    }
}

void Undo::copyBlocks(std::list<Block *> *dst, std::list<Block *> *src)
{
    for (std::list<Block*>::iterator lit = dst->begin(); lit!= dst->end(); lit++)
        delete *lit;
    dst->clear();
    for (std::list<Block*>::iterator lit = src->begin(); lit!= src->end(); lit++)
    {
        Block* block = *lit;
        Block* newBlock;
        if (block->bombed())
            newBlock = new BombBlock(*((BombBlock*)block));
        else if (block->lifted())
            newBlock = new LiftBlock (*((LiftBlock*)block));
        else
            newBlock = new Block(*block);
        dst->push_back(newBlock);
    }

}

void Undo::saveBlocks()
{
    State& st = *states[ind];
    copyBlocks(&st.blocks, &field->blocks);
}

void Undo::saveLifts()
{
    State& st = *states[ind];
    copyLifts(&st.lifts, &field->lifts);
}

void Undo::restoreBlocks()
{
    State& st = *states[ind];
    copyBlocks(&field->blocks ,&st.blocks);
    for (std::list<Block*>::iterator bit = field->blocks.begin(); bit != field->blocks.end(); bit++)
        (*bit)->doStop();
}

void Undo::restoreLifts()
{
    State& st = *states[ind];
    copyLifts(&field->lifts ,&st.lifts);
}


void Undo::restore()
{
    if (len<=0)
        return;
    field->levelDone = false;
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
    restoreLifts();
    restoreBlocks();
    field->runner->armored = st.armored;
    field->clearBombs();
    field->runner->setX(st.runner.x);
    field->runner->setY(st.runner.y);
    field->runner->setVX(0);
    field->runner->setVY(0);
    field->runner->climbing = false;

    field->nRunnerBombs = st.nRunnerBombs;
    field->nRunnerGrenades = st.nRunnerGrenades;
    field->nRunnerKeys = st.nRunnerKeys;
    field->runner->calcNearBlocks();
    ind = (ind+size-1) % size;
}




