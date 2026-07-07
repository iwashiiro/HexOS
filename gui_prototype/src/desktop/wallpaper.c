#include "desktop/wallpaper.h"

void Wallpaper_Create(
    Surface *surface)
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

            int pattern =
                (x + y) % 4;

            switch (pattern)
            {
            case 0:
                cell->glyph = 0x2588;
                break;

            case 1:
                cell->glyph = 0x2593;
                break;

            case 2:
                cell->glyph = 0x2592;
                break;

            case 3:
                cell->glyph = 0x2591;
                break;
            }

            cell->foreground.r = 180;
            cell->foreground.g = 180;
            cell->foreground.b = 180;

            cell->background.r = 20;
            cell->background.g = 20;
            cell->background.b = 20;

            cell->transparent = 0;
        }
    }
}