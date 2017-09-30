#ifndef _AI__AI_H
#define _AI__AI_H

#include "../game/game.h"

void ai_init(PLAYER player);
void ai_fini(PLAYER player);

void ai_bind_to_game(PLAYER player);
void ai_unbind_from_game(PLAYER player);

#endif
