#ifndef _GAME__BOARD_H
#define _GAME__BOARD_H

#include "../common.h"

#define BOARD_WIDTH     (7)
#define BOARD_HEIGHT    (6)

typedef struct board_t
{
    /* amount of tokens in each column */
    uint8_t token_count[BOARD_WIDTH];

    /* token board */
    uint8_t board[BOARD_WIDTH][BOARD_HEIGHT];
} board_t;

#include "game.h"

void board_init(board_t* board);
void board_fini(board_t* board);

uint8_t board_check_for_winner(board_t* board);

#define board_square(board, row, column)        ((board)->board[column][row])

#define board_is_column_full(board, column)     ((board)->token_count[column] >= BOARD_HEIGHT)
#define board_is_column_empty(board, column)    ((board)->token_count[column] == 0)

static void board_push(board_t* board, uint8_t column, PLAYER player)
{
    if (board_is_column_full(board, column))
        return;
    board->board[column][5 - board->token_count[column]++] = player;
}

static void board_pop(board_t* board, uint8_t column)
{
    if (board_is_column_empty(board, column))
        return;
    board->board[column][5 - --board->token_count[column]] = PLAYER_NONE;
}

/*
#define board_push(board, column, player) \
    do {                                                                                \
        if (board_is_column_full((board), (column))) break;                             \
        (board)->board[(column)][5 - (board)->token_count[(column)]++] = player;        \
    } while (0)
#define board_pop(board, column) \
    do {                                                                                \
        if (board_is_column_empty((board), (column))) break;                            \
        (board)->board[(column)][5 - --(board)->token_count[(column)]] = PLAYER_NONE;   \
    } while (0)
*/

#endif
