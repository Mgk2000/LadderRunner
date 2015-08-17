#include "undo.h"
#include "play.h"
#include "cell.h"
#include "runner.h"
#include "bomb.h"
#include "block.h"
#include "logmsg.h"

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
    LOGD("Save ind=%d", ind);
    State& st = *states[ind];
//    st.clear();
    st.runner = Coords(field->runner->x, field->runner->y);
    st.nRunnerBombs = field->nRunnerBombs;
    st.nRunnerKeys = field->nRunnerKeys;
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
}

void Undo::restore()
{
    if (len<=0)
        return;
    len--;
    LOGD("Restore ind=%d", ind);
    State& st = *states[ind];
//    clearField();
    for (int i =0; i< field->nrows; i++)
        for (int j = 0; j< field->ncols; j++)
            field->cells[j+ i* field->ncols]->setKind((Texture::Kind)st.cells[j+ i* field->ncols]);
    std::list<Coords>::iterator cit = st.blocks.begin();
    for (std::list<Block*> ::iterator bit = field->blocks.begin(); bit != field->blocks.end(); bit++, cit++)
    {
        Block * block = *bit;
        Coords c = *cit;
        block->setX(c.x);
        block->setY(c.y);
    }
    field->runner->setX(st.runner.x);
    field->runner->setY(st.runner.y);
    field->nRunnerBombs = st.nRunnerBombs;
    field->nRunnerKeys = st.nRunnerKeys;
    ind = (ind+size-1) % size;
}


