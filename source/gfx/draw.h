#ifndef _GFX__DRAW_H
#define _GFX__DRAW_H

#include "../common.h"
#include <graphx.h>

#define FONT_HEIGHT 8

typedef enum PALETTE_COLOR
{
    BLACK       = 0, /* #000000 */
    WHITE       = 1, /* #ffffff */
    RED         = 2, /* #ff2020 */
    BLUE        = 3, /* #3030ff */
    GREEN       = 4, /* #0cf00c */
    DARK_GRAY   = 5, /* #201010 */
    LIGHT_GRAY  = 6, /* #505050 */
} PALETTE_COLOR;

void draw_init(void);
void draw_fini(void);

/* text drawing methods */
#define draw_str_set_fgcolor    gfx_SetTextFGColor
#define draw_str_set_bgcolor    gfx_SetTextBGColor
#define draw_str_set_transcolor gfx_SetTextTransparentColor

#define draw_str_get_width      gfx_GetStringWidth
#define draw_str                gfx_PrintStringXY
static void draw_str_hcenter(const char* str, uint8_t y)
{
    gfx_PrintStringXY(str, (LCD_WIDTH - gfx_GetStringWidth(str)) >> 1, y);
}
static void draw_str_uint(unsigned int n, uint8_t len, uint24_t x, uint8_t y)
{
    gfx_SetTextXY(x, y);
    gfx_PrintUInt(n, len);
}

/* screen drawing methods */
#define draw_scr_fill           gfx_FillScreen

/* other drawing methods */
#define draw_set_color          gfx_SetColor
#define draw_rectangle          gfx_Rectangle_NoClip
#define draw_fill_rectangle     gfx_FillRectangle_NoClip

/* draw buffer methods */
#define draw_buf_swap           gfx_SwapDraw
#define draw_buf_blit           gfx_BlitBuffer

#endif
