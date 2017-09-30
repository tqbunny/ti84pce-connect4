#include "board.h"
#include <string.h>

void board_init(board_t* board)
{
    memset(board, 0, sizeof(board_t));
}

void board_fini(board_t* board)
{
    ;
}

uint8_t board_check_for_winner(board_t* board)
{
    int8_t row;
    uint8_t column, player;

    for (row = BOARD_HEIGHT - 1; row >= 0; row--)
    {
        for (column = 0; column < BOARD_WIDTH; column++)
        {
            player = board_square(board, row, column);
            if (player == PLAYER_NONE)
                continue;

            /* horizontal */
            if (column < BOARD_WIDTH - 3)
            {
                if (player == board_square(board, row, column + 1)
                &&  player == board_square(board, row, column + 2)
                &&  player == board_square(board, row, column + 3))
                {
                    return player;
                }
            }

            /* vertical */
            if (row < BOARD_HEIGHT - 3)
            {
                if (player == board_square(board, row + 1, column)
                &&  player == board_square(board, row + 2, column)
                &&  player == board_square(board, row + 3, column))
                {
                    return player;
                }
            }

            /* diagonal right-up */
            if (row >= 3 && column < BOARD_WIDTH - 3)
            {
                if (player == board_square(board, row - 1, column + 1)
                &&  player == board_square(board, row - 2, column + 2)
                &&  player == board_square(board, row - 3, column + 3))
                {
                    return player;
                }
            }

            /* diagonal left-up */
            if (row >= 3 && column >= 3)
            {
                if (player == board_square(board, row - 1, column - 1)
                &&  player == board_square(board, row - 2, column - 2)
                &&  player == board_square(board, row - 3, column - 3))
                {
                    return player;
                }
            }
        }
    }

    return PLAYER_NONE;
}
