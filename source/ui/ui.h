#ifndef _UI__UI_H
#define _UI__UI_H

#include "../common.h"
#include "menu.h"

typedef enum UI_MODE
{
    UI_NONE,
    UI_MENU,
    UI_GAME,
} UI_MODE;

void ui_set_mode(UI_MODE mode);
void ui_main(void);

#endif
