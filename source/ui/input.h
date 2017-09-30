#ifndef _UI__INPUT_H
#define _UI__INPUT_H

#include "../common.h"
#include <keypadc.h>

extern kb_key_t _key_group_1;
extern kb_key_t _key_group_2;
extern kb_key_t _key_group_6;
extern kb_key_t _key_group_7;

void input_scan(void);

#define input_is_2nd_pressed()      (_key_group_1 & kb_2nd)
#define input_is_mode_pressed()     (_key_group_1 & kb_Mode)
#define input_is_del_pressed()      (_key_group_1 & kb_Del)
#define input_is_alpha_pressed()    (_key_group_2 & kb_Alpha)
#define input_is_clear_pressed()    (_key_group_6 & kb_Clear)
#define input_get_arrows()          (_key_group_7)

#define input_scan_while(condition) do input_scan(); while (condition)

#define KEY_UP                      (kb_Up)
#define KEY_DOWN                    (kb_Down)
#define KEY_LEFT                    (kb_Left)
#define KEY_RIGHT                   (kb_Right)

#endif
