#include "menu_players.h"

#include <stdio.h>

#include "draw.h"
#include "text.h"

scene_t *menu_players_scene(void)
{
	scene_t *s = malloc(sizeof(scene_t));

	s->init = &menu_players_init;
	s->update = &menu_players_update;
	s->draw = &menu_players_draw;

	return s;
}

void menu_players_init(game_state_t *state)
{
	(void)state;
	/* anything to do here? */
}

int menu_players_update(game_state_t *state)
{
	(void)state;
	return state->time < 2.0f;
}

void menu_players_draw(game_state_t *state)
{
	draw_background(state);

	char str[2];

	print_text(state, "PLAYER COLOURS", &(vector2_t){116, 150});

	for (int i = 0; i < state->player_count; i++) {
		sprintf(str, "%i", i + 1);
		print_text(state, str, &(vector2_t){116, 160 + (i + 1) * 30});

		rect_t rect = {(vector2_t){156, 160 + (i + 1) * 30},
						(vector2_t){20, 20} };

		draw_rect(state, &rect, &state->player[i].color);
	}

	print_text(state, "PRESS L TO START", &(vector2_t){66, 484});
}
