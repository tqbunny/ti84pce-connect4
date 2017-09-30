#ifndef _UI__GAME_H
#define _UI__GAME_H

#include "../common.h"
#include "../gfx/draw.h"
#include "../game/game.h"

typedef enum GAME_TYPE
{
    GAME_PLAYER_AGAINST_AI,
    GAME_AI_AGAINST_AI,
    GAME_PLAYER_AGAINST_PLAYER,
} GAME_TYPE;

PALETTE_COLOR ui_game_get_player_color(PLAYER player);
void ui_game_map_player_color(PLAYER player, PALETTE_COLOR color);
void ui_game_set_type(GAME_TYPE type);
void ui_game_loop(void);

#endif
