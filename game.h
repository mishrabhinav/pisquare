#ifndef GAME_H
#define GAME_H

#include "gamestate.h"

scene_t *game_scene(void);
void game_init(game_state_t *state);
int game_update(game_state_t *state);
void game_draw(game_state_t *state);
void game_free(game_state_t *state);

#endif
