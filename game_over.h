#ifndef GAME_OVER
#define GAME_OVER

#include "gamestate.h"

scene_t *game_over_scene(void);
void game_over_init(game_state_t *state);
int game_over_update(game_state_t *state);
void game_over_draw(game_state_t *state);

#endif
