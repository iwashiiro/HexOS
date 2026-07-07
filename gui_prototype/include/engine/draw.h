#ifndef ENGINE_DRAW_H
#define ENGINE_DRAW_H

#include "engine/surface.h"

void Draw_Char(
    Surface *surface,
    int x,
    int y,
    Cell cell);

void Draw_Text(
    Surface *surface,
    int x,
    int y,
    const char *text,
    Cell cell);

void Draw_Rect(
    Surface *surface,
    int x,
    int y,
    int width,
    int height,
    Cell cell);

#endif