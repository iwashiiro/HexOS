#include "graphics/font.h"

static Glyph CreateEmptyGlyph(void)
{
    Glyph glyph;

    glyph.codepoint = 0;

    for (int i = 0; i < GLYPH_HEIGHT; i++)
    {
        glyph.bitmap[i] = 0;
    }

    return glyph;
}

Glyph Font_GetGlyph(
    uint32_t codepoint)
{
    Glyph glyph = CreateEmptyGlyph();

    glyph.codepoint = codepoint;

    switch (codepoint)
    {

        /*
            █ FULL BLOCK
        */

    case 0x2588:

        for (int i = 0; i < GLYPH_HEIGHT; i++)
        {
            glyph.bitmap[i] = 0xFF;
        }

        break;

        /*
            ▓ DARK SHADE
        */

    case 0x2593:

        glyph.bitmap[0] = 0xAA;
        glyph.bitmap[1] = 0x55;
        glyph.bitmap[2] = 0xAA;
        glyph.bitmap[3] = 0x55;
        glyph.bitmap[4] = 0xAA;
        glyph.bitmap[5] = 0x55;
        glyph.bitmap[6] = 0xAA;
        glyph.bitmap[7] = 0x55;

        break;

        /*
            ▒ MEDIUM SHADE
        */

    case 0x2592:

        glyph.bitmap[0] = 0xCC;
        glyph.bitmap[1] = 0x33;
        glyph.bitmap[2] = 0xCC;
        glyph.bitmap[3] = 0x33;
        glyph.bitmap[4] = 0xCC;
        glyph.bitmap[5] = 0x33;
        glyph.bitmap[6] = 0xCC;
        glyph.bitmap[7] = 0x33;

        break;

        /*
            ░ LIGHT SHADE
        */

    case 0x2591:

        glyph.bitmap[0] = 0x88;
        glyph.bitmap[1] = 0x22;
        glyph.bitmap[2] = 0x88;
        glyph.bitmap[3] = 0x22;
        glyph.bitmap[4] = 0x88;
        glyph.bitmap[5] = 0x22;
        glyph.bitmap[6] = 0x88;
        glyph.bitmap[7] = 0x22;

        break;

    default:

        break;
    }

    return glyph;
}