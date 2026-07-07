#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>

#include "desktop/layer.h"
#include "desktop/wallpaper.h"

#include "graphics/renderer.h"

int main(void)
{
    Renderer renderer;

    if (!Renderer_Init(
            &renderer,
            800,
            600))
    {
        return 1;
    }

    /*
        Layer 0:
        Wallpaper
    */

    Layer wallpaper;

    if (!Layer_Init(
            &wallpaper,
            50,
            30,
            0))
    {
        Renderer_Shutdown(
            &renderer);

        return 1;
    }

    /*
        Layer 1:
        Icone desktop
    */

    Layer icons;

    if (!Layer_Init(
            &icons,
            50,
            30,
            1))
    {
        Layer_Destroy(
            &wallpaper);

        Renderer_Shutdown(
            &renderer);

        return 1;
    }

    /*
        Creazione wallpaper
    */

    Wallpaper_Create(
        &wallpaper.surface);

    /*
        Creazione layer icone vuoto
    */

    Surface_Clear(
        &icons.surface,
        ' ');

    /*
        Disegniamo una falsa icona
        di test
    */

    for (int y = 10;
         y < 15;
         y++)
    {
        for (int x = 20;
             x < 25;
             x++)
        {
            Cell *cell =
                Surface_GetCell(
                    &icons.surface,
                    x,
                    y);

            cell->glyph = 0x2588;

            cell->foreground.r = 255;
            cell->foreground.g = 255;
            cell->foreground.b = 255;

            cell->transparent = 0;
        }
    }

    int running = 1;

    while (running)
    {

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
        }

        SDL_SetRenderDrawColor(
            renderer.renderer,
            20,
            20,
            20,
            255);

        SDL_RenderClear(
            renderer.renderer);

        /*
            Rendering dei layer

            Ordine:
            basso -> alto
        */

        Renderer_DrawSurface(
            &renderer,
            &wallpaper.surface,
            2);

        Renderer_DrawSurface(
            &renderer,
            &icons.surface,
            2);

        SDL_RenderPresent(
            renderer.renderer);
    }

    Layer_Destroy(
        &icons);

    Layer_Destroy(
        &wallpaper);

    Renderer_Shutdown(
        &renderer);

    return 0;
}