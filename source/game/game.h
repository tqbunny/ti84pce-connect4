#ifndef _GAME__GAME_H
#define _GAME__GAME_H

#include "../common.h"

typedef enum PLAYER
{
    PLAYER_NONE = 0,
    PLAYER_RHO = 1,
    PLAYER_PHI = 2,
} PLAYER;

#include "board.h"

typedef int8_t (*player_bind_play_turn_t)(PLAYER player, board_t* board);

typedef struct player_binds_t
{
    /* returns the column the player places a token in */
    player_bind_play_turn_t play_turn;
} player_binds_t;

void game_init(void);
void game_fini(void);

void game_bind_player(PLAYER player, player_binds_t* binds);
void game_unbind_player(PLAYER player);

board_t* game_get_board(void);
bool game_has_ended(void);
PLAYER game_get_winner(void);
PLAYER game_get_turn_player(void);
uint8_t game_get_turn(void);

void game_start(void);
void game_play_step(void);

#endif
