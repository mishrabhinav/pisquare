#include "menu.h"

#include <stdio.h>

#include "player.h"
#include "text.h"
#include "draw.h"

scene_t *menu_scene(void)
{
	scene_t *s = malloc(sizeof(scene_t));

	s->init = &menu_init;
	s->update = &menu_update;
	s->draw = &menu_draw;

	return s;
}

void menu_init(game_state_t *state)
{
	state->player_count = 1;
}

int menu_update(game_state_t *state)
{
	if (RPI_GetGpioValue(PLAYER_1_RIGHT) == 0)
		return 0;


	if (RPI_GetGpioValue(PLAYER_1_LEFT) == 0)
		state->player_count = (state->player_count) % 4 + 1;

	return 1;
}

void menu_draw(game_state_t *state)
{
	char player_num[8];

	draw_background(state);

	print_text(state, "MENU",
			&(vector2_t){221, 161});
	print_text(state, "L : SELECT    R : START",
			&(vector2_t){36, 484});

	for (int i = 0; i < 4; i++) {
		sprintf(player_num, "%d PLAYER", i + 1);
		print_text(state, player_num,
				&(vector2_t){181, 201 + 30 * i});
	}

	sprintf(player_num, "%d PLAYER", state->player_count);
	print_text_color(state, player_num,
		&(vector2_t){181, 201 + 30 * (state->player_count - 1)},
		&(color_t){0, 255, 0, 255});

	draw_square(state,
		&(vector2_t){160, 208 + 30 * (state->player_count - 1)},
				14, (int)(360 * state->time/0.5f) % 360,
					&(color_t){0, 255, 0, 255});
}
