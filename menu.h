#ifndef MENU
#define MENU

#include "gamestate.h"


scene_t *menu_scene(void);
void menu_init(game_state_t *state);
int menu_update(game_state_t *state);
void menu_draw(game_state_t *state);

#endif
