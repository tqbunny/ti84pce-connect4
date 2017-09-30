#include "ai.h"
#include "../debug.h"
#include <stdlib.h>
#include <string.h>

/* ai simulation settings, tweak for different behavior */
#define SCORE_INSTANT_WIN   (300000)
#define SCORE_INSTANT_LOSS  (-300000)
#define SCORE_WIN           (3000)
#define SCORE_DRAW          (2500)
#define SCORE_LOSS          (-8000)

typedef struct ai_move_t
{
    int8_t column;
    int24_t score;
} ai_move_t;

typedef struct ai_t
{
    PLAYER player;
    PLAYER opponent;

    uint8_t possible_moves_count;
    ai_move_t possible_moves[BOARD_WIDTH];
    ai_move_t* best_move;

    board_t* simboard;
} ai_t;

static ai_t player_ai[2];
#define ai_get_from_player(player)  (&player_ai[player - 1])

void ai_init(PLAYER player)
{
    ai_t* ai = ai_get_from_player(player);
    ai->player = player;
    ai->opponent = player == PLAYER_RHO ? PLAYER_PHI : PLAYER_RHO;
    ai->possible_moves_count = 0;
    ai->best_move = NULL;

    ai->simboard = malloc(sizeof(board_t));
}

void ai_fini(PLAYER player)
{
    ai_t* ai = ai_get_from_player(player);

    if (ai->simboard != NULL)
        free(ai->simboard);
    else
        dprintf("ai_fini(): ai->simboard is already NULL for player %u!\n", player);

    ai->simboard = NULL;
}

bool ai_push_possible_move(ai_t* ai, ai_move_t* move)
{
    if (ai->possible_moves_count < BOARD_WIDTH)
    {
        memcpy(&ai->possible_moves[ai->possible_moves_count++], move, sizeof(ai_move_t));
        return true;
    }

    return false;
}

void ai_keep_best_moves(ai_t* ai)
{
    uint8_t i, possible_moves_count = 0;
    ai_move_t* best_move = NULL;
    ai_move_t possible_moves[BOARD_WIDTH];

    /* select best move */
    for (i = 0; i < ai->possible_moves_count; i++)
    {
        if (best_move == NULL)
        {
            dprintf("ai_keep_best_moves(): set initial best move #%i at [%i] with score %i\n",
                i, ai->possible_moves[i].column, ai->possible_moves[i].score);
            best_move = &ai->possible_moves[i];
        }
        else if (best_move->score < ai->possible_moves[i].score)
        {
            dprintf("ai_keep_best_moves(): set best move #%i at [%i] with score %i\n",
                i, ai->possible_moves[i].column, ai->possible_moves[i].score);
            best_move = &ai->possible_moves[i];
        }
        else
        {
            dprintf("ai_keep_best_moves(): rejecting move #%i at [%i] with score %i\n",
                i, ai->possible_moves[i].column, ai->possible_moves[i].score);
        }
    }

    if (best_move == NULL)
        return;

    for (i = 0; i < ai->possible_moves_count; i++)
    {
        if (ai->possible_moves[i].score >= best_move->score)
        {
            dprintf("ai_keep_best_moves(): keeping move #%i at [%i] with score %i\n",
                i, ai->possible_moves[i].column, ai->possible_moves[i].score);
            memcpy(&possible_moves[possible_moves_count++], &ai->possible_moves[i],
                sizeof(ai_move_t));
        }
    }

    memcpy(ai->possible_moves, possible_moves, sizeof(possible_moves));
    ai->possible_moves_count = possible_moves_count;
}

/* round_count should always start from at least 1
 * round_count 0 is "simple move simulation", and is not simulated in this function
 */
void ai_simulate_move_deep(ai_t* ai, ai_move_t* move, uint8_t round_count, uint8_t column)
{
    int24_t added_score = 0;
    PLAYER winner, playing = (round_count % 2 == 0) ? ai->player : ai->opponent;

    board_push(ai->simboard, column, playing);
    winner = board_check_for_winner(ai->simboard);
    if (winner == PLAYER_NONE)
    {
        uint8_t i;

        added_score = SCORE_DRAW / round_count;

        for (i = 0; i < BOARD_WIDTH; i++)
            if (!board_is_column_full(ai->simboard, i) && round_count < 3)
                ai_simulate_move_deep(ai, move, round_count + 1, i);
    }
    else
    {
        if (winner == ai->player)
            added_score = SCORE_WIN / round_count;
        else
            added_score = SCORE_LOSS / round_count;
    }
    move->score += added_score;
    board_pop(ai->simboard, column);
}

bool ai_simulate_move(ai_t* ai, ai_move_t* move, bool deep)
{
    uint8_t column = move->column;
    PLAYER winner;

    board_push(ai->simboard, column, ai->player);
    winner = board_check_for_winner(ai->simboard);
    if (winner == PLAYER_NONE)
    {
        uint8_t i;

        move->score += SCORE_DRAW;

        /* only do deep simulations if asked so */
        if (deep)
        {
            for (i = 0; i < BOARD_WIDTH; i++)
            {
                if (!board_is_column_full(ai->simboard, i))
                {
                    dprintf("ai_simulate_move(): deep simulation at [%u] for move at [%i]\n", i,
                        column);
                    ai_simulate_move_deep(ai, move, 1, i);
                }
            }
        }
    }
    else
    {
        if (winner == ai->player)
        {
            dprintf("ai_simulate_move(): instant win for move at [%i]\n", column);
            move->score += SCORE_INSTANT_WIN;
            ai->best_move = move;
            return true;
        }
        else
        {
            dprintf("ai_simulate_move(): instant loss for move at [%i]\n", column);
            move->score += SCORE_INSTANT_LOSS;
        }
    }
    board_pop(ai->simboard, column);
    return false;
}

int8_t ai_play_turn(PLAYER player, board_t* board)
{
    int8_t column, i;
    ai_t* ai = ai_get_from_player(player);

    dprintf("ai_play_turn(): playing as player %u\n", player);

    /* reset possible moves */
    ai->possible_moves_count = 0;
    ai->best_move = NULL;

    for (column = 0; column < BOARD_WIDTH; column++)
    {
        ai_move_t move;

        if (board_is_column_full(board, column))
            continue;

        memset(&move, 0, sizeof(ai_move_t));
        move.column = column;
        ai_push_possible_move(ai, &move);
        dprintf("ai_play_turn(): added possible move for [%i]\n", column);
    }

    /* prepare simulation board */
    memcpy(ai->simboard, board, sizeof(board_t));

    /* simulate "simple" moves */
    for (i = 0; i < ai->possible_moves_count; i++)
    {
        dprintf("ai_play_turn(): simulating non-deep move #%i\n", i);
        ai_simulate_move(ai, &ai->possible_moves[i], false);
        dprintf("ai_play_turn(): move #%i scores %i\n", i, ai->possible_moves[i].score);

        /* if a "perfect move" was found, stop searching */
        if (ai->best_move != NULL)
        {
            dprintf("ai_play_turn(): found perfect move at [%i]\n", ai->best_move->column);
_found_best_move:
            return ai->best_move->column;
        }
    }

    /* simulate "deep" ("complex") moves */
    for (i = 0; i < ai->possible_moves_count; i++)
    {
        dprintf("ai_play_turn(): simulating deep move #%i\n", i);
        ai_simulate_move(ai, &ai->possible_moves[i], true);
        dprintf("ai_play_turn(): move #%i scores %i\n", i, ai->possible_moves[i].score);
    }

    ai_keep_best_moves(ai);
    if (ai->possible_moves_count > 0)
    {
        uint8_t selected = rand() % ai->possible_moves_count;
        dprintf("ai_play_turn(): %i equivalent best moves, playing #%i\n",
            ai->possible_moves_count, selected);
        return ai->possible_moves[selected].column;
    }

    /* we are not supposed to reach this point */
    dprintf("ai_play_turn(): [!] playing random column! this should not happen!\n");
    return rand() % BOARD_WIDTH;
}

void ai_bind_to_game(PLAYER player)
{
    player_binds_t binds;
    binds.play_turn = ai_play_turn;

    game_bind_player(player, &binds);
}

void ai_unbind_from_game(PLAYER player)
{
    game_unbind_player(player);
}
