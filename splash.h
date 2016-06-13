#ifndef SPLASH
#define SPLASH

#include "gamestate.h"

scene_t *splash_scene(void);
void splash_init(game_state_t *state);
int splash_update(game_state_t *state);
void splash_draw(game_state_t *state);

#endif
