#include "config.h"
#include <fileioc.h>
#include "../debug.h"
#include "../i18n/i18n.h"
#include "../ui/game.h"

#define CONFIG_VERSION  (1U)

typedef struct config_t
{
    uint16_t version;
    I18N_LANG i18n_lang;
    PALETTE_COLOR player_color;
    /* TODO: ai difficulty ? */
} config_t;

static config_t cfg;

static void config_apply(void)
{
    i18n_set_current_language(cfg.i18n_lang);
    ui_game_map_player_color(PLAYER_RHO, cfg.player_color);
}

static void config_prepare(void)
{
    cfg.i18n_lang = i18n_get_current_language();
    cfg.player_color = ui_game_get_player_color(PLAYER_RHO);
}

void config_init(void)
{
    config_load();
}

void config_load(void)
{
    ti_var_t var;

    ti_CloseAll();
    var = ti_Open("Conn4Cfg", "r");
    if (var)
    {
        ti_Read(&cfg, sizeof(config_t), 1, var);
        if (cfg.version == CONFIG_VERSION)
        {
            config_apply();
            dprintf("config_load(): Configuration was loaded.\n");
        }
        else
        {
            dprintf("config_load(): Configuration version from file does not match.\n");
        }
    }
    else
    {
        dprintf("config_load(): No configuration file found.\n");
    }
    ti_CloseAll();
}

void config_save(void)
{
    ti_var_t var;

    cfg.version = CONFIG_VERSION;
    config_prepare();

    ti_CloseAll();
    var = ti_Open("Conn4Cfg", "w");
    if (var)
    {
        ti_Write(&cfg, sizeof(config_t), 1, var);
        ti_SetArchiveStatus(true, var); /* archive the cfg appvar */
        dprintf("config_save(): Configuration file was successfully written.\n");
    }
    else
    {
        dprintf("config_save(): Could not write configuration file.\n");
    }
    ti_CloseAll();
}

void config_fini(void)
{
    ;
}
