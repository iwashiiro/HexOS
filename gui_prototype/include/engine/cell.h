#ifndef CELL_H
#define CELL_H

#include <stdint.h>

#include "engine/color.h"

typedef struct Cell
{
    uint32_t glyph;

    Color foreground;

    Color background;

    int transparent;

} Cell;

Cell Cell_Create(
    uint32_t glyph);

#endif