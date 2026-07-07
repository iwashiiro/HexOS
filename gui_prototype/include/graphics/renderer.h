#ifndef GRAPHICS_RENDERER_H
#define GRAPHICS_RENDERER_H

#include <SDL2/SDL.h>

#include "graphics/glyph.h"

#include "engine/cell.h"
#include "engine/surface.h"

typedef struct Renderer
{
    SDL_Window *window;

    SDL_Renderer *renderer;

} Renderer;

int Renderer_Init(
    Renderer *renderer,
    int width,
    int height);

void Renderer_Shutdown(
    Renderer *renderer);

void Renderer_DrawGlyph(
    Renderer *renderer,
    Glyph *glyph,
    int x,
    int y,
    int scale);

void Renderer_DrawCell(
    Renderer *renderer,
    Cell *cell,
    int x,
    int y,
    int scale);

void Renderer_DrawSurface(
    Renderer *renderer,
    Surface *surface,
    int scale);

#endif