#include "game.h"
#include "../debug.h"
#include <stdlib.h>
#include <string.h>

static player_binds_t player_binds[2];
static board_t* board = NULL;
static bool ended = false;
static PLAYER winner = PLAYER_NONE;
static PLAYER turn_player;
static uint8_t turn;

void game_init(void)
{
    if (board != NULL)
        dprintf("game_init(): board isn't NULL !\n");

    board = malloc(sizeof(board_t));
    board_init(board);

    memset(&player_binds, 0, sizeof(player_binds));

    ended = false;
    winner = PLAYER_NONE;
    turn = 0;
}

void game_fini(void)
{
    board_fini(board);

    if (board != NULL)
        free(board);
    else
        dprintf("game_fini(): board is already NULL !\n");

    board = NULL;
}

static player_binds_t* game_get_player_binds(PLAYER player)
{
    return &player_binds[player - 1];
}

void game_bind_player(PLAYER player, player_binds_t* binds)
{
    memcpy(game_get_player_binds(player), binds, sizeof(player_binds_t));
}

void game_unbind_player(PLAYER player)
{
    memset(game_get_player_binds(player), 0, sizeof(player_binds_t));
}

board_t* game_get_board(void)
{
    return board;
}

bool game_has_ended(void)
{
    return ended;
}

PLAYER game_get_winner(void)
{
    return winner;
}

PLAYER game_get_turn_player(void)
{
    return turn_player;
}

uint8_t game_get_turn(void)
{
    return turn;
}

void game_start(void)
{
    turn_player = (PLAYER)((rand() % 2) + 1);
    turn = 1;
}

static void game_declare_winner(PLAYER player)
{
    ended = true;
    turn_player = PLAYER_NONE;
    winner = player;
}

void game_play_step(void)
{
    int8_t col;
    uint8_t i, win = PLAYER_NONE;
    player_binds_t* binds;

    if (ended)
    {
        dprintf("game_play_step(): trying to play a step but game ended !\n");
        return;
    }

    dprintf("game_play_step(): step is played by player %u\n", turn_player);

    binds = game_get_player_binds(turn_player);
    col = binds->play_turn(turn_player, board);
    if (col < 0)
    {
        dprintf("game_play_step(): player %u skipped step\n", turn_player);
        return; /* wait for the next step to play this turn instead */
    }

    if (col < BOARD_WIDTH)
        board_push(board, col, turn_player);
    else
        dprintf("game_play_step(): invalid column %u from player %u !\n", col, turn_player);

    /* check for a winner */
    win = board_check_for_winner(board);
    if (win != PLAYER_NONE)
        game_declare_winner(win);

    if (turn >= BOARD_WIDTH * BOARD_HEIGHT)
        game_declare_winner(PLAYER_NONE);
    else
    {
        turn_player = (turn_player == PLAYER_RHO) ? PLAYER_PHI : PLAYER_RHO;
        turn++;
    }
}
