#include "engine/draw.h"

void Draw_Char(
    Surface *surface,
    int x,
    int y,
    Cell cell)
{
    Surface_SetCell(
        surface,
        x,
        y,
        cell);
}

void Draw_Text(
    Surface *surface,
    int x,
    int y,
    const char *text,
    Cell cell)
{
    int offset = 0;

    while (text[offset] != '\0')
    {
        Surface_SetCell(
            surface,
            x + offset,
            y,
            cell);

        offset++;
    }
}

void Draw_Rect(
    Surface *surface,
    int x,
    int y,
    int width,
    int height,
    Cell cell)
{
    for (int iy = 0; iy < height; iy++)
    {
        for (int ix = 0; ix < width; ix++)
        {
            Surface_SetCell(
                surface,
                x + ix,
                y + iy,
                cell);
        }
    }
}