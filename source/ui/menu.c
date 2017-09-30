#include "menu.h"
#include "ui.h"
#include "game.h"
#include "input.h"
#include "../gfx/draw.h"
#include "../i18n/i18n.h"
#include "../storage/config.h"

MENU menu_current = MENU_NONE;

static uint8_t cursor_position = 0;

void menu_set_current(MENU menu)
{
    menu_current = menu;
    cursor_position = 0;  /* reset cursor position */
}

static void menu_draw_opt(uint8_t opt, const char* str)
{
    if (cursor_position == opt)
        draw_str_set_fgcolor(GREEN);
    else
        draw_str_set_fgcolor(BLACK);

    draw_str_hcenter(str, 100 + FONT_HEIGHT * opt);
    draw_str_set_fgcolor(BLACK);
}

static void menu_draw_opt_with_arg(uint8_t opt, const char* str, const char* arg)
{
    uint24_t str_width, total_width, start_pos;

    str_width = draw_str_get_width(str);
    total_width = str_width + draw_str_get_width(arg);
    start_pos = (LCD_WIDTH - total_width) >> 1;

    if (cursor_position == opt)
        draw_str_set_fgcolor(GREEN);
    else
        draw_str_set_fgcolor(BLACK);

    draw_str(str, start_pos, 100 + FONT_HEIGHT * opt);
    draw_str(arg, start_pos + str_width, 100 + FONT_HEIGHT * opt);
    draw_str_set_fgcolor(BLACK);
}

static void menu_draw_footer(void)
{
    draw_str_set_fgcolor(LIGHT_GRAY);
    draw_str(VERSION_STRING, 2, 212);
    draw_str("@tqbun, 2017", 2, 220);
    draw_str(SOURCE_URI, 2, 230);
    draw_str_set_fgcolor(BLACK);
}

static void menu_draw_main(void)
{
    draw_scr_fill(WHITE);

    draw_str_set_fgcolor(BLUE);
    draw_str_hcenter(i18n.game_title, 20);
    draw_str_hcenter(i18n.game_subtitle, 28);
    draw_str_hcenter(i18n.menu_main, 40);
    draw_str_set_fgcolor(BLACK);

    menu_draw_opt(0, i18n.play);
    menu_draw_opt(1, i18n.menu_settings);
    menu_draw_opt(2, i18n.menu_quit);

    menu_draw_footer();
    draw_buf_swap();
}

static void menu_draw_settings(void)
{
    draw_scr_fill(WHITE);

    draw_str_set_fgcolor(BLUE);
    draw_str_hcenter(i18n.game_title, 20);
    draw_str_hcenter(i18n.game_subtitle, 28);
    draw_str_hcenter(i18n.menu_settings, 40);
    draw_str_set_fgcolor(BLACK);

    menu_draw_opt_with_arg(0, i18n.language,        /* Language */
        i18n_get_language_name(i18n_get_current_language()));
    menu_draw_opt_with_arg(1, i18n.player_color,    /* Player color */
        ui_game_get_player_color(PLAYER_RHO) == RED ? i18n.red : i18n.blue);
    menu_draw_opt(2, i18n.back);

    menu_draw_footer();
    draw_buf_swap();
}

static void menu_loop_main(void)
{
    bool running = true;

_redraw:
    menu_draw_main();

    do
    {
        input_scan();

        if (input_is_2nd_pressed())
        {
            switch (cursor_position)
            {
                case 0: /* Play */
                    if (input_is_alpha_pressed())
                        ui_game_set_type(GAME_AI_AGAINST_AI);
                    else
                        ui_game_set_type(GAME_PLAYER_AGAINST_AI);

                    ui_set_mode(UI_GAME);

                    /* leave menu loop to enter game ui loop */
                    menu_set_current(MENU_NONE);
                    running = false;

                    input_scan_while(input_is_2nd_pressed());
                    break;
                case 1: /* Settings */
                    menu_set_current(MENU_SETTINGS);
                    running = false;
                    break;
                case 2: /* Quit */
                    goto _quit;
                    break;
            }

            input_scan_while(input_is_2nd_pressed());
        }
        else if (input_is_clear_pressed())
        {
_quit:
            menu_set_current(MENU_NONE);
            ui_set_mode(UI_NONE);
            running = false;

            input_scan_while(input_is_clear_pressed() || input_is_2nd_pressed());
        }
        else if (input_get_arrows() & KEY_DOWN)
        {
            if (++cursor_position > 2)
                cursor_position = 0;

            input_scan_while(input_get_arrows() & KEY_DOWN);
            goto _redraw;
        }
        else if (input_get_arrows() & KEY_UP)
        {
            if (cursor_position-- == 0)
                cursor_position = 2;

            input_scan_while(input_get_arrows() & KEY_UP);
            goto _redraw;
        }
    }
    while (running);
}

static void menu_loop_settings(void)
{
    bool running = true;

_redraw:
    menu_draw_settings();

    do
    {
        input_scan();

        if (input_is_2nd_pressed())
        {
            switch (cursor_position)
            {
            case 0: /* Language */
                {
                    uint8_t lang = (uint8_t)i18n_get_current_language();
                    if (++lang >= I18N_LANGS)
                        lang = 0;

                    i18n_set_current_language((I18N_LANG)lang);
                    break;
                }
            case 1: /* Player color */
                {
                    PALETTE_COLOR color = ui_game_get_player_color(PLAYER_RHO);
                    color = color == RED ? BLUE : RED;
                    ui_game_map_player_color(PLAYER_RHO, color);
                    break;
                }
            case 2: /* Back */
                goto _go_to_main_menu;
                break;
            }

            input_scan_while(input_is_2nd_pressed());
            goto _redraw;
        }
        else if (input_is_clear_pressed())
        {
_go_to_main_menu:
            config_save();

            menu_set_current(MENU_MAIN);
            running = false;

            input_scan_while(input_is_clear_pressed() || input_is_2nd_pressed());
        }
        else if (input_get_arrows() & KEY_DOWN)
        {
            if (++cursor_position > 2)
                cursor_position = 0;

            input_scan_while(input_get_arrows() & KEY_DOWN);
            goto _redraw;
        }
        else if (input_get_arrows() & KEY_UP)
        {
            if (cursor_position-- == 0)
                cursor_position = 2;

            input_scan_while(input_get_arrows() & KEY_UP);
            goto _redraw;
        }
    }
    while (running);
}

void menu_loop(void)
{
    bool running = true;

    input_scan_while(input_is_2nd_pressed());

    do
    {
        switch (menu_current)
        {
        case MENU_MAIN:     menu_loop_main();       break;
        case MENU_SETTINGS: menu_loop_settings();   break;

        case MENU_NONE:
        default:
            running = false;
            break;
        }
    }
    while (running);
}
