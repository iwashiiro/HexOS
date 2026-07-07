#include "graphics/renderer.h"
#include "graphics/font.h"

int Renderer_Init(
    Renderer *renderer,
    int width,
    int height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return 0;
    }

    renderer->window =
        SDL_CreateWindow(
            "Halbschatten",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            0);

    if (renderer->window == NULL)
    {
        SDL_Quit();
        return 0;
    }

    renderer->renderer =
        SDL_CreateRenderer(
            renderer->window,
            -1,
            SDL_RENDERER_ACCELERATED);

    if (renderer->renderer == NULL)
    {
        SDL_DestroyWindow(
            renderer->window);

        SDL_Quit();

        return 0;
    }

    return 1;
}

void Renderer_DrawGlyph(
    Renderer *renderer,
    Glyph *glyph,
    int x,
    int y,
    int scale)
{
    if (renderer == NULL ||
        glyph == NULL)
    {
        return;
    }

    for (int row = 0;
         row < GLYPH_HEIGHT;
         row++)
    {
        uint8_t line =
            glyph->bitmap[row];

        for (int col = 0;
             col < GLYPH_WIDTH;
             col++)
        {
            if (line & (1 << (7 - col)))
            {
                SDL_Rect pixel;

                pixel.x =
                    x + col * scale;

                pixel.y =
                    y + row * scale;

                pixel.w =
                    scale;

                pixel.h =
                    scale;

                SDL_RenderFillRect(
                    renderer->renderer,
                    &pixel);
            }
        }
    }
}

void Renderer_DrawCell(
    Renderer *renderer,
    Cell *cell,
    int x,
    int y,
    int scale)
{
    if (renderer == NULL ||
        cell == NULL)
    {
        return;
    }

    /*
        Le celle trasparenti
        non cancellano il layer sotto
    */

    if (cell->transparent)
    {
        return;
    }

    /*
        Disegno dello sfondo
    */

    SDL_SetRenderDrawColor(
        renderer->renderer,
        cell->background.r,
        cell->background.g,
        cell->background.b,
        255);

    SDL_Rect background;

    background.x = x;
    background.y = y;

    background.w =
        GLYPH_WIDTH * scale;

    background.h =
        GLYPH_HEIGHT * scale;

    SDL_RenderFillRect(
        renderer->renderer,
        &background);

    /*
        Disegno del glyph
    */

    Glyph glyph =
        Font_GetGlyph(
            cell->glyph);

    SDL_SetRenderDrawColor(
        renderer->renderer,
        cell->foreground.r,
        cell->foreground.g,
        cell->foreground.b,
        255);

    Renderer_DrawGlyph(
        renderer,
        &glyph,
        x,
        y,
        scale);
}

void Renderer_DrawSurface(
    Renderer *renderer,
    Surface *surface,
    int scale)
{
    if (renderer == NULL ||
        surface == NULL)
    {
        return;
    }

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

            Renderer_DrawCell(
                renderer,
                cell,
                x * GLYPH_WIDTH * scale,
                y * GLYPH_HEIGHT * scale,
                scale);
        }
    }
}

void Renderer_Shutdown(
    Renderer *renderer)
{
    if (renderer == NULL)
    {
        return;
    }

    if (renderer->renderer != NULL)
    {
        SDL_DestroyRenderer(
            renderer->renderer);
    }

    if (renderer->window != NULL)
    {
        SDL_DestroyWindow(
            renderer->window);
    }

    SDL_Quit();
}