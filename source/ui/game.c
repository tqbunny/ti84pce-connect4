#include "game.h"
#include "ui.h"
#include "input.h"
#include "../gfx/draw.h"
#include "../i18n/i18n.h"
#include "../game/game.h"
#include "../ai/ai.h"

#define BOARD_X (2)
#define BOARD_Y (LCD_HEIGHT - 2 - (22 * BOARD_HEIGHT + 2))

static bool game_loop_running;
static uint8_t cursor_position;
static board_t* board;
static GAME_TYPE game_type = GAME_PLAYER_AGAINST_AI;

static PALETTE_COLOR color_mappings[3] = {
    WHITE,  /* PLAYER_NONE */
    RED,    /* PLAYER_RHO */
    BLUE,   /* PLAYER_PHI */
};

static void ui_game_draw(bool showCursor);

PALETTE_COLOR ui_game_get_player_color(PLAYER player)
{
    return color_mappings[player];
}

void ui_game_map_player_color(PLAYER player, PALETTE_COLOR color)
{
    PLAYER opposite_player = player == PLAYER_RHO ? PLAYER_PHI : PLAYER_RHO;
    PALETTE_COLOR opposite_color = color == RED ? BLUE : RED;

    color_mappings[player] = color;
    color_mappings[opposite_player] = opposite_color;
}

int8_t ui_game_play_turn(PLAYER player, board_t* board)
{
    kb_key_t prev_arrows;
    bool prev_2nd = true;
    bool waiting_for_input = true;

    input_scan_while(input_is_2nd_pressed());

    do
    {
        ui_game_draw(true);

        /* only take key presses */
        prev_arrows = input_get_arrows();
        input_scan_while(input_get_arrows() & prev_arrows || (input_is_2nd_pressed() && prev_2nd));
        prev_2nd = false;

        if (input_is_clear_pressed())
        {
            /* TODO: game saving ? */
            game_loop_running = false;
            return -1;
        }

        if (input_get_arrows() & KEY_LEFT)
        {
            if (cursor_position-- == 0)
                cursor_position = 6;
        }
        else if (input_get_arrows() & KEY_RIGHT)
        {
            if (++cursor_position > 6)
                cursor_position = 0;
        }

        if (input_is_2nd_pressed())
        {
            if (!board_is_column_full(board, cursor_position))
            {
                waiting_for_input = false;
                return cursor_position;
            }
        }
    }
    while (waiting_for_input);

    return cursor_position;
}

void ui_game_bind(PLAYER player)
{
    player_binds_t binds;
    binds.play_turn = ui_game_play_turn;

    cursor_position = 3;    /* middle of the board */
    game_bind_player(player, &binds);
}

void ui_game_unbind(PLAYER player)
{
    game_unbind_player(player);
}

void ui_game_draw_board(bool showCursor)
{
    uint8_t row, column;

    draw_set_color(BLACK);
    draw_fill_rectangle(BOARD_X, BOARD_Y, 22 * BOARD_WIDTH + 2, 22 * BOARD_HEIGHT + 2);

    for (row = 0; row < BOARD_HEIGHT; row++)
    {
        for (column = 0; column < BOARD_WIDTH; column++)
        {
            PLAYER p = board_square(board, row, column);
            if (showCursor && column == cursor_position && p == PLAYER_NONE)
                draw_set_color(LIGHT_GRAY);
            else
                draw_set_color(color_mappings[p]);

            draw_fill_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + row * 22, 20, 20);
        }
    }
}

void ui_game_draw_cursor(bool showCursor)
{
    if (showCursor)
        draw_str("VV", BOARD_X + 4 + cursor_position * 22, BOARD_Y - 10);
}

void ui_game_draw_info(void)
{
    PLAYER turn_player;
    uint24_t turn_str_width;

    turn_str_width = draw_str_get_width(i18n.turn);

    draw_str(i18n.turn, BOARD_X + 22 * BOARD_WIDTH + 4, LCD_HEIGHT - 10);
    draw_str_uint(game_get_turn(), 2, BOARD_X + 22 * BOARD_WIDTH + 4 + turn_str_width,
        LCD_HEIGHT - 10);

    turn_player = game_get_turn_player();
    if (color_mappings[turn_player] == RED)
        draw_str(i18n.red, BOARD_X + 22 * BOARD_WIDTH + 4, LCD_HEIGHT - 18);
    else if (color_mappings[turn_player] == BLUE)
        draw_str(i18n.blue, BOARD_X + 22 * BOARD_WIDTH + 4, LCD_HEIGHT - 18);
}

void ui_game_draw_match_end_highlight(void)
{
    int8_t row;
    uint8_t column, player;

    draw_set_color(GREEN);
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
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + row * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + (column + 1) * 22, BOARD_Y + 2 + row * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + (column + 2) * 22, BOARD_Y + 2 + row * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + (column + 3) * 22, BOARD_Y + 2 + row * 22, 20, 20);
                }
            }

            /* vertical */
            if (row < BOARD_HEIGHT - 3)
            {
                if (player == board_square(board, row + 1, column)
                &&  player == board_square(board, row + 2, column)
                &&  player == board_square(board, row + 3, column))
                {
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + row * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + (row + 1) * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + (row + 2) * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + (row + 3) * 22, 20, 20);
                }
            }

            /* diagonal right-up */
            if (row >= 3 && column < BOARD_WIDTH - 3)
            {
                if (player == board_square(board, row - 1, column + 1)
                &&  player == board_square(board, row - 2, column + 2)
                &&  player == board_square(board, row - 3, column + 3))
                {
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + row * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + (column + 1) * 22, BOARD_Y + 2 + (row - 1) * 22,
                        20, 20);
                    draw_rectangle(BOARD_X + 2 + (column + 2) * 22, BOARD_Y + 2 + (row - 2) * 22,
                        20, 20);
                    draw_rectangle(BOARD_X + 2 + (column + 3) * 22, BOARD_Y + 2 + (row - 3) * 22,
                        20, 20);
                }
            }

            /* diagonal left-up */
            if (row >= 3 && column >= 3)
            {
                if (player == board_square(board, row - 1, column - 1)
                &&  player == board_square(board, row - 2, column - 2)
                &&  player == board_square(board, row - 3, column - 3))
                {
                    draw_rectangle(BOARD_X + 2 + column * 22, BOARD_Y + 2 + row * 22, 20, 20);
                    draw_rectangle(BOARD_X + 2 + (column - 1) * 22, BOARD_Y + 2 + (row - 1) * 22,
                        20, 20);
                    draw_rectangle(BOARD_X + 2 + (column - 2) * 22, BOARD_Y + 2 + (row - 2) * 22,
                        20, 20);
                    draw_rectangle(BOARD_X + 2 + (column - 3) * 22, BOARD_Y + 2 + (row - 3) * 22,
                        20, 20);
                }
            }
        }
    }
}

void ui_game_draw_match_end(void)
{
    const char* str = NULL;
    PLAYER winner = PLAYER_NONE;

    if (game_has_ended())
    {
        winner = game_get_winner();
        if (winner == PLAYER_NONE)
            str = i18n.draw;
        else if (color_mappings[winner] == RED)
            str = i18n.red_wins;
        else if (color_mappings[winner] == BLUE)
            str = i18n.blue_wins;

        draw_str_hcenter(str, 30);
        draw_str_hcenter(i18n.press_clear, 38);

        if (winner != PLAYER_NONE)
            ui_game_draw_match_end_highlight();
    }
}

static void ui_game_draw(bool showCursor)
{
    draw_scr_fill(WHITE);

    ui_game_draw_board(showCursor);
    ui_game_draw_cursor(showCursor);

    ui_game_draw_info();
    ui_game_draw_match_end();

    draw_buf_swap();
}

void ui_game_set_type(GAME_TYPE type)
{
    game_type = type;
}

void ui_game_loop(void)
{
    game_loop_running = true;

    game_init();

    switch (game_type)
    {
    case GAME_PLAYER_AGAINST_AI:
        ai_init(PLAYER_PHI);
        ai_bind_to_game(PLAYER_PHI);
        ui_game_bind(PLAYER_RHO);
        break;

    case GAME_AI_AGAINST_AI:
        ai_init(PLAYER_PHI);
        ai_init(PLAYER_RHO);
        ai_bind_to_game(PLAYER_PHI);
        ai_bind_to_game(PLAYER_RHO);
        break;

    default:
        break;
    }

    game_start();
    board = game_get_board();

    do
    {
        ui_game_draw(false);
        game_play_step();
        ui_game_draw(false);

        if (game_has_ended())
        {
            game_loop_running = false;
            input_scan_while(!input_is_clear_pressed());
        }
    }
    while (game_loop_running);

    switch (game_type)
    {
    case GAME_PLAYER_AGAINST_AI:
        ui_game_unbind(PLAYER_RHO);
        ai_unbind_from_game(PLAYER_PHI);
        ai_fini(PLAYER_PHI);
        break;

    case GAME_AI_AGAINST_AI:
        ai_unbind_from_game(PLAYER_RHO);
        ai_unbind_from_game(PLAYER_PHI);
        ai_fini(PLAYER_RHO);
        ai_fini(PLAYER_PHI);
        break;

    default:
        break;
    }

    game_fini();
    ui_set_mode(UI_NONE);
}
