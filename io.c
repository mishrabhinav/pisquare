#include "io.h"

void io_update(game_state_t *state)
{
	state->p1_right_debounce_timer += state->delta;
	state->p1_left_debounce_timer += state->delta;

	state->p1_right_down = !RPI_GetGpioValue(PLAYER_1_RIGHT);
	state->p1_left_down = !RPI_GetGpioValue(PLAYER_1_LEFT);

	state->p1_right_released |= !state->p1_right_down;
	state->p1_left_released |= !state->p1_left_down;

	if (state->p1_right_down
			&& state->p1_right_debounce_timer > BUTTON_DEBOUNCE) {
		state->p1_right_debounce_timer = 0;
		state->p1_right_held = 1;
		if (state->p1_right_released) {
			state->p1_right_released = 0;
			state->p1_right_clicked = 1;
		}
	}

	if (state->p1_left_down
			&& state->p1_left_debounce_timer > BUTTON_DEBOUNCE) {
		state->p1_left_debounce_timer = 0;
		state->p1_left_held = 1;
		state->p1_left_clicked = 1;

		if (state->p1_left_released) {
			state->p1_left_released = 0;
			state->p1_left_clicked = 1;
		}
	}
}

void io_reset(game_state_t *state)
{
	state->p1_right_debounce_timer = 0;
	state->p1_left_debounce_timer = 0;
	state->p1_right_clicked = 0;
	state->p1_left_clicked = 0;
	state->p1_left_held = 0;
	state->p1_right_held = 0;
	state->p1_right_released = 0;
	state->p1_left_released = 0;
}

void io_handle_click_right(game_state_t *state)
{
	state->p1_right_clicked = 0;
}

void io_handle_click_left(game_state_t *state)
{
	state->p1_left_clicked = 0;
}

void io_handle_hold_right(game_state_t *state)
{
	state->p1_right_debounce_timer = 0;
	state->p1_right_held = 0;
}

void io_handle_hold_left(game_state_t *state)
{
	state->p1_left_debounce_timer = 0;
	state->p1_left_held = 0;
}
