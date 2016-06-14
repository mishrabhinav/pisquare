#include "game_over.h"
#include "draw.h"
#include "text.h"
#include "io.h"

#include <stdio.h>

scene_t *game_over_scene(void)
{
	scene_t *s = malloc(sizeof(scene_t));

	s->init = &game_over_init;
	s->update = &game_over_update;
	s->draw = &game_over_draw;

	return s;
}

void game_over_init(game_state_t *state)
{
	(void)state;
}

int game_over_update(game_state_t *state)
{
	(void)state;
	if (state->time > 1.0f)
		if (state->p1_right_clicked) {
			io_handle_click_right(state);
			return 0;
		}

	return 1;
}

void game_over_draw(game_state_t *state)
{
	draw_background(state);
	print_text(state, "GAME OVER", &(vector2_t){165, 181});

	char str[16];

	if (state->player_count == 1) {
		sprintf(str, "TIME: %.1f", state->timer_game);
		print_text(state, str, &(vector2_t){145, 231});
	} else {
		color_t col;

		for (int i = 0; i < state->player_count; i++)
			if (state->player[i].lives > 0) {
				col = get_color(i + 1);
				sprintf(str, "PLAYER %i WINS!", i + 1);
			}
		print_text_color(state, str, &(vector2_t){136, 231}, &col);
	}
	if (state->time > 1.0f)
		print_text(state, "PRESS R TO RESTART", &(vector2_t){66, 484});
}
