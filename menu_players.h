#ifndef MENU_PLAYERS
#define MENU_PLAYERS

#include "gamestate.h"

scene_t *menu_players_scene(void);
void menu_players_init(game_state_t *state);
int menu_players_update(game_state_t *state);
void menu_players_draw(game_state_t *state);

#endif
