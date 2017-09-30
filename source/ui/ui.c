#include "ui.h"
#include "game.h"

UI_MODE ui_mode = UI_NONE;

void ui_set_mode(UI_MODE mode)
{
    ui_mode = mode;
}

void ui_main(void)
{
    bool running = true;

    do
    {
        switch (ui_mode)
        {
        case UI_MENU:   menu_loop();    break;
        case UI_GAME:   ui_game_loop(); break;

        case UI_NONE:
        default:
            running = false;
            break;
        }
    }
    while (running);
}
