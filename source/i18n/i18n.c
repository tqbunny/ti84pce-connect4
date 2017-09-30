#include "i18n.h"

const char* i18n_lang_names[] = {
    "English (UK)",
    "Francais (France)",
};

I18N_LANG i18n_language = EN_UK;    /* default language */

i18n_t i18n_list[2] = {
    /* English (UK) */
    {
        /* .game_title      */ "Connect 4",
        /* .game_subtitle   */ "For TI-84 Plus",

        /* .menu_main       */ "Main Menu",
        /* .menu_settings   */ "Settings",
        /* .menu_credits    */ "Credits",
        /* .menu_quit       */ "Quit",

        /* .play            */ "Play",
        /* .red             */ "Red",
        /* .blue            */ "Blue",
        /* .back            */ "Back",
        /* .cancel          */ "Cancel",
        /* .turn            */ "Turn #",

        /* .language        */ "Language: ",
        /* .player_color    */ "Player color: ",

        /* .red_wins        */ "Red player wins !",
        /* .blue_wins       */ "Blue player wins !",
        /* .draw            */ "Draw !",

        /* .press_clear     */ "Press [Clear] to exit",
    },
    /* French (France) */
    {
        /* .game_title      */ "Puissance 4",
        /* .game_subtitle   */ "Pour TI-83 Premium CE",

        /* .menu_main       */ "Menu Principal",
        /* .menu_settings   */ "Parametres",
        /* .menu_credits    */ "Credits",
        /* .menu_quit       */ "Quitter",

        /* .play            */ "Jouer",
        /* .red             */ "Rouge",
        /* .blue            */ "Bleu",
        /* .back            */ "Retour",
        /* .cancel          */ "Annuler",
        /* .turn            */ "Tour #",

        /* .language        */ "Langue: ",
        /* .player_color    */ "Couleur du joueur: ",

        /* .red_wins        */ "Le joueur rouge gagne !",
        /* .blue_wins       */ "Le joueur bleu gagne !",
        /* .draw            */ "Egalite !",

        /* .press_clear     */ "Appuyez sur [Annul] pour quitter",
    },
};
