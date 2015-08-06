#include "runner.h"

Runner::Runner(Field* _field) : MovingObject(_field, Texture::RUNNER)
{
    v = 3.0;
}
