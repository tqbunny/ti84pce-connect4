#ifndef _I18N__I18N_H
#define _I18N__I18N_H

#define I18N_LANGS  (2)

typedef enum I18N_LANG
{
    EN_UK = 0,
    FR_FR = 1,
} I18N_LANG;

typedef struct i18n_t
{
    const char* game_title;
    const char* game_subtitle;

    const char* menu_main;
    const char* menu_settings;
    const char* menu_credits;
    const char* menu_quit;

    const char* play;
    const char* red;
    const char* blue;
    const char* back;
    const char* cancel;
    const char* turn;

    const char* language;
    const char* player_color;

    const char* red_wins;
    const char* blue_wins;
    const char* draw;

    const char* press_clear;
} i18n_t;

extern const char* i18n_lang_names[];
extern I18N_LANG i18n_language;
extern i18n_t i18n_list[];

#define i18n_set_current_language(lang) do { i18n_language = lang; } while (0)
#define i18n_get_current_language()     (i18n_language)
#define i18n_get_language_name(lang)    (i18n_lang_names[lang])

#define i18n    i18n_list[i18n_language]

#endif
