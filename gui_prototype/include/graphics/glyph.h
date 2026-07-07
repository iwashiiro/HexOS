#ifndef GRAPHICS_GLYPH_H
#define GRAPHICS_GLYPH_H

#include <stdint.h>

#define GLYPH_WIDTH 8
#define GLYPH_HEIGHT 8

typedef struct Glyph
{
    uint32_t codepoint;

    uint8_t bitmap[GLYPH_HEIGHT];

} Glyph;

#endif