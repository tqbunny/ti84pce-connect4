#include "common.h"
#include "debug.h"
#include "gfx/draw.h"
#include "ui/ui.h"
#include "storage/config.h"

void main(void)
{
    dprintf("main(): Game started.\n");

    /* seed randomness generation */
    srand(rtc_Time());

    draw_init();
    config_init();

    menu_set_current(MENU_MAIN);
    ui_set_mode(UI_MENU);
    ui_main();

    config_fini();
    draw_fini();
    dprintf("main(): Game terminated.\n");
}
