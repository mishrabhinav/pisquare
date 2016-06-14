#include "game.h"

#include <stdio.h>
#include "libarmc/rpi-gpio.h"
#include "rpi-random.h"
#include "renderer.h"
#include "random.h"
#include "text.h"
#include "box.h"
#include "player.h"
#include "update.h"
#include "draw.h"

scene_t *game_scene(void)
{
	scene_t *s = malloc(sizeof(scene_t));

	s->init = &game_init;
	s->update = &game_update;
	s->draw = &game_draw;
	s->free = &game_free;

	return s;
}

static void add_box(game_state_t *state)
{
	box_t *box = box_new();

	regenerate_box(state, box);

	++state->boxes_count;
	state->boxes = realloc(state->boxes,
					state->boxes_count * sizeof(box_t));

	state->boxes[state->boxes_count-1] = *box;
}

static bullet_t *add_bullet(game_state_t *state)
{
	/* search for unused bullets */
	bullet_t *bullet = bullet_new();

	++state->bullets_count;
	state->bullets = realloc(state->bullets,
				state->bullets_count * sizeof(bullet_t));

	state->bullets[state->bullets_count-1] = *bullet;
	return bullet;
}

static int collides(entity_t *e1, entity_t *e2)
{
	if (e1->pos.x < e2->pos.x + e2->size.x
		&& e1->pos.x + e1->size.x > e2->pos.x
		&& e1->pos.y < e2->pos.y + e2->size.y
		&& e1->pos.y + e1->size.y > e2->pos.y)
		return 1;
	return 0;
}

static void print_time(game_state_t *state)
{
	char str[10];
	int len = sprintf(str, "%.1f", state->timer_game);

	int x = state->device->width - len * 20;

	print_text(state, str, &(vector2_t){x, 486});
}

static void print_lives(game_state_t *state)
{
	char str[8];

	print_text_color(state, "LIVES:", &(vector2_t){0, 486},
			 &(color_t){255, 255, 255, 255});

	for (int i = 0; i < state->player_count; i++) {
		sprintf(str, "%d", state->player[i].lives);
		print_text_color(state, str, &(vector2_t){120 + i * 30, 486},
				&state->player[i].color);
	}
}

static void print_fps(game_state_t *state)
{
	char str[2];

	sprintf(str, "%d", state->bullets_count);
	print_text_color(state, str, &(vector2_t){320, 486},
			 &(color_t){0, 255, 0, 255});
}

void game_init(game_state_t *state)
{
	RPI_InitRandom();
	/* game area */
	state->area = (vector2_t){state->device->width,
				  state->device->height - 32};

	/* boxes */
	state->boxes_count = 0;

	/* bullets */
	state->bullets_count = 0;

	/* player */
	for (int i = 0; i < state->player_count; i++) {
		player_t *player = player_new();

		player->entity->pos = get_pos(i+1);
		player->color = get_color(i + 1);
		player->right_pin = get_right_pin(i + 1);
		player->left_pin = get_left_pin(i + 1);

		RPI_SetGpioInput(player->right_pin);
		RPI_SetGpioInput(player->left_pin);

		state->player[i] = *player;
	}

	for (int i = 0; i < BOX_COUNT_MAX/8; i++)
		add_box(state);

	/* timers */
	state->timer_box = 0;
	state->timer_game = 0;
	state->timer_frame = 0;
}

int game_update(game_state_t *state)
{
	/* Timers */
	state->timer_game += state->delta;
	state->timer_box += state->delta;
	state->timer_frame += state->delta;

	/* Diagnostics */
	if (state->timer_frame >= 1.f) {
		state->fps = state->frames_count;
		state->frames_count = 0;
		state->timer_frame = 0;
	}

	/* Boxes */
	/* Increase number of boxes over time */
	if (state->timer_box > BOX_TIMER
				&& state->boxes_count < BOX_COUNT_MAX) {
		state->timer_box = 0;
		add_box(state);
	}
	/* Update Boxes */
	for (int i = 0; i < state->boxes_count; i++)
		update_box(state, &state->boxes[i]);

	/* Update Bullets */
	for (int i = 0; i < state->bullets_count; i++)
		if (!state->bullets[i].dead)
			update_bullet(state, &state->bullets[i]);

	/* Player */
	for (int i = 0; i < state->player_count; i++) {
		if (state->player[i].lives > 0) {
			update_player(state, &state->player[i]);
			if (state->player[i].shoot) {
				player_shoot(&state->player[i],
							add_bullet(state));
			}
		}
	}

	/* Collision Detection */
	/* Players with boxes */
	for (int j = 0; j < state->player_count; j++) {
		if (state->player[j].lives > 0
				&& state->player[j].debounce_time
						> PLAYER_DEBOUNCE_TIME) {
			/* Boxes */
			for (int i = 0; i < state->boxes_count; i++) {
				if (collides(state->player[j].entity,
						state->boxes[i].entity)) {
					regenerate_box(state, &state->boxes[i]);
					player_injure(&state->player[j]);
				}
			}
		}
	}
	/* Bullets with Players/Boxes */
	for (int i = 0; i < state->bullets_count; i++) {
		/* Players */
		if (!state->bullets[i].dead) {
			for (int j = 0; j < state->player_count; j++) {
				if (state->player[j].lives > 0
					&& state->player[j].debounce_time
					> PLAYER_DEBOUNCE_TIME
					&& collides(state->player[j].entity,
						state->bullets[i].entity)) {
					player_injure(&state->player[j]);
					state->bullets[i].dead = 1;
				}
			}

			/* Boxes */
			for (int j = 0; j < state->boxes_count; j++) {
				if (collides(state->boxes[j].entity,
						state->bullets[i].entity)) {
					regenerate_box(state, &state->boxes[j]);
					state->bullets[i].dead = 1;
				}
			}
		}
	}

	/* End Condition */
	int alive = state->player_count;

	for (int i = 0; i < state->player_count; i++)
		if (state->player[i].lives <= 0)
			alive--;

	return !!(state->player_count - 1) ? alive > 1 : alive > 0;
}

void game_draw(game_state_t *state)
{
	/* background */
	draw_background(state);

	/* boxes */
	for (int i = 0; i < state->boxes_count; i++)
		draw_box(state, &state->boxes[i]);

	/* bullets */
	for (int i = 0; i < state->bullets_count; i++)
		if (!state->bullets[i].dead)
			draw_bullet(state, &state->bullets[i]);

	/* player */
	for (int i = 0; i < state->player_count; i++)
		if (state->player[i].lives > 0)
			draw_player(state, &state->player[i]);

	/* UI */
	print_time(state);
	print_lives(state);
	print_fps(state);

	/* Diagnostics */
	state->frames_count += 1;
}


void game_free(game_state_t *state)
{
	/* boxes */
	for (int i = 0; i < state->boxes_count; i++)
		box_free(&state->boxes[i]);

	free(state->boxes);
	free(state->player);
}
