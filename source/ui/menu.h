#ifndef _UI__MENU_H
#define _UI__MENU_H

typedef enum MENU
{
    MENU_NONE,
    MENU_MAIN,
    MENU_SETTINGS,
} MENU;

void menu_set_current(MENU menu);
void menu_loop(void);

#endif
