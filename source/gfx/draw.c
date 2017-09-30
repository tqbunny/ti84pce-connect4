#include "draw.h"

void draw_init(void)
{
    gfx_Begin(gfx_8bpp);
    gfx_SetDrawBuffer();

    /* palette setup */
    gfx_palette[BLACK]      = gfx_RGBTo1555(0, 0, 0);
    gfx_palette[WHITE]      = gfx_RGBTo1555(255, 255, 255);
    gfx_palette[RED]        = gfx_RGBTo1555(255, 32, 32);
    gfx_palette[BLUE]       = gfx_RGBTo1555(48, 48, 240);
    gfx_palette[GREEN]      = gfx_RGBTo1555(12, 240, 12);
    gfx_palette[DARK_GRAY]  = gfx_RGBTo1555(32, 16, 16);
    gfx_palette[LIGHT_GRAY] = gfx_RGBTo1555(80, 80, 80);

    draw_str_set_transcolor(WHITE);
    draw_str_set_bgcolor(WHITE);
    draw_str_set_fgcolor(BLACK);
}

void draw_fini(void)
{
    gfx_End();
}
