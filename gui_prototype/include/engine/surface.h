#ifndef SURFACE_H
#define SURFACE_H

#include "engine/cell.h"

typedef struct Surface
{
    int width;

    int height;

    Cell *cells;

} Surface;

int Surface_Init(
    Surface *surface,
    int width,
    int height);

void Surface_Destroy(
    Surface *surface);

Cell *Surface_GetCell(
    Surface *surface,
    int x,
    int y);

void Surface_SetCell(
    Surface *surface,
    int x,
    int y,
    Cell cell);

void Surface_Clear(
    Surface *surface,
    uint32_t glyph);

#endif