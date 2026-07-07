#include <stdlib.h>

#include "engine/surface.h"

int Surface_Init(
    Surface *surface,
    int width,
    int height)
{
    surface->width = width;

    surface->height = height;

    surface->cells =
        malloc(
            sizeof(Cell) *
            width *
            height);

    if (surface->cells == NULL)
    {
        return 0;
    }

    Surface_Clear(
        surface,
        ' ');

    return 1;
}

Cell *Surface_GetCell(Surface *surface, int x, int y)
{
    if (x < 0 || y < 0 || x >= surface->width || y >= surface->height)
    {
        return NULL;
    }

    return &surface->cells[y * surface->width + x];
}

void Surface_SetCell(Surface *surface, int x, int y, Cell cell)
{
    if (x < 0 || y < 0 || x >= surface->width || y >= surface->height)
    {
        return;
    }

    surface->cells[y * surface->width + x] = cell;
}

void Surface_Clear(
    Surface *surface,
    uint32_t glyph)
{
    for (int y = 0;
         y < surface->height;
         y++)
    {
        for (int x = 0;
             x < surface->width;
             x++)
        {
            Cell *cell =
                Surface_GetCell(
                    surface,
                    x,
                    y);

            cell->glyph = glyph;

            cell->foreground.r = 255;
            cell->foreground.g = 255;
            cell->foreground.b = 255;

            cell->background.r = 0;
            cell->background.g = 0;
            cell->background.b = 0;

            /*
                Se la cella contiene uno spazio,
                significa che non deve disegnare nulla.
            */

            if (glyph == ' ')
            {
                cell->transparent = 1;
            }
            else
            {
                cell->transparent = 0;
            }
        }
    }
}

void Surface_Destroy(Surface *surface)
{
    if (surface->cells != NULL)
    {
        free(surface->cells);
    }

    surface->cells = NULL;
}