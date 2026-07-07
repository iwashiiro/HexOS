#include "engine/cell.h"

Cell Cell_Create(
    uint32_t glyph)
{
    Cell cell;

    cell.glyph = glyph;

    cell.foreground.r = 255;
    cell.foreground.g = 255;
    cell.foreground.b = 255;

    cell.background.r = 0;
    cell.background.g = 0;
    cell.background.b = 0;

    cell.transparent = 0;

    return cell;
}