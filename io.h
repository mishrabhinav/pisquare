#ifndef IO
#define IO

#include "gamestate.h"
#include "player.h"

#define BUTTON_DEBOUNCE 0.2f

void io_reset(game_state_t *state);
void io_update(game_state_t *state);
void io_handle_click_right(game_state_t *state);
void io_handle_click_left(game_state_t *state);
void io_handle_hold_right(game_state_t *state);
void io_handle_hold_left(game_state_t *state);

#endif
