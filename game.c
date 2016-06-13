#include "game.h"

#include <stdio.h>
#include "libarmc/rpi-gpio.h"
#include "rpi-random.h"
#include "renderer.h"
#include "random.h"
#include "text.h"
#include "box.h"
#include "player.h"
#include "move.h"
#include "draw.h"

static void add_box(game_state_t *state)
{
	int up = random_int(2);
	int forward = random_int(2);
	int sign = 2 * forward - 1;
	float vel = sign * (random_int(50) + 10);

	box_t *box = box_new();

	int randy = random_int(state->area.y - box->entity->size.y)
						+ box->entity->size.y/2;
	int randx = random_int(state->area.x - box->entity->size.x)
						+ box->entity->size.x/2;

	int posx = !up * (-box->entity->size.x + !forward * (state->area.x
						+ box->entity->size.x))
						+ up * randx;
	int posy = up * (-box->entity->size.y + !forward * (state->area.y
						+ box->entity->size.y))
						+ !up * randy;
	(void)vel;

	int grey = random_int(128) + 127;

	box->entity->size = (vector2_t){12, 12};
	box->entity->pos = (vector2_t){posx, posy};
	box->entity->vel = (vector2_t){!up * vel, up * vel};
	box->color = (color_t){grey, grey, grey, 255};

	++state->boxes_count;
	state->boxes = realloc(state->boxes,
					state->boxes_count * sizeof(box_t));

	state->boxes[state->boxes_count-1] = *box;
}

static int will_collide(player_t *player, box_t *box)
{
	if (player->entity->pos.x < box->entity->pos.x + box->entity->size.x &&
		player->entity->pos.x + player->entity->size.x >
						box->entity->pos.x &&
		player->entity->pos.y <
				box->entity->pos.y + box->entity->size.y &&
		player->entity->pos.y + player->entity->size.y >
						box->entity->pos.y)
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
	char str[10];

	sprintf(str, "HEALTH %d", state->player->lives);
	print_text_color(state, str, &(vector2_t){0, 486},
			 &(color_t){ .r = 255, .g = 0, .b = 0, .a = 255});
}

static void print_io(game_state_t *state)
{
	char str[3];

	str[0] = RPI_GetGpioValue(state->player[0].right_pin) > 0 ? '1' : '0';
	str[1] = RPI_GetGpioValue(state->player[0].left_pin) > 0 ? '1' : '0';
	str[2] = 0;

	(void) str;

	/* print_text(state, str, (vector2_t){200, 486}); */
}

static void print_fps(game_state_t *state)
{
	char str[2];

	sprintf(str, "%d", state->fps);
	print_text_color(state, str, &(vector2_t){250, 486},
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

	/* player */
	for (int i = 0; i < state->player_count; i++) {
		player_t *player = player_new();

		player->entity->pos = get_pos(i+1);
		player->angular_vel = 0;
		player->color = get_colour(i + 1);
		player->right_pin = get_right_pin(i + 1);
		player->left_pin = get_left_pin(i + 1);

		RPI_SetGpioInput(player->right_pin);
		RPI_SetGpioInput(player->left_pin);

		state->player[i] = *player;
	}

	for (int i = 0; i < BOX_COUNT_MAX; i++)
		add_box(state);

	/* timers */
	state->timer_box = 0;
	state->timer_game = 0;
}

void game_menu(game_state_t *state)
{
	char player_num[8];

	state->player_count = 1;

	while (RPI_GetGpioValue(PLAYER_1_RIGHT) != 0) {
		draw_background(state);
		print_text(state, "MENU",
				(vector2_t){221, 161});
		print_text(state, "L : SELECT    R : START",
				(vector2_t){36, 484});

		if (RPI_GetGpioValue(PLAYER_1_LEFT) == 0)
			state->player_count = (state->player_count) % 4 + 1;

		for (int i = 0; i < 4; i++) {
			sprintf(player_num, "%d PLAYER", i + 1);
			print_text(state, player_num,
					(vector2_t){181, 201 + 30 * i});
		}

		sprintf(player_num, "%d PLAYER", state->player_count);
		print_text_color(state, player_num,
			(vector2_t){181, 201 + 30 * (state->player_count - 1)},
			(color_t){0, 255, 0, 255});

		graphics_flush(state->device);
	}
}

void game_update(game_state_t *state)
{
	/* Timers */
	state->timer_box += state->delta;
	state->timer_game += state->delta;
	state->timer_frame += state->delta;

	/* Detect collisions */
	for (int j = 0; j < state->player_count; j++) {
		for (int i = 0; i < state->boxes_count; i++) {
			if (state->player[j].lives > 0)
				if (will_collide(&state->player[j],
							&state->boxes[i]))
					state->player[j].lives--;
		}
	}

	/* Diagnostics */
	if (state->timer_frame >= 1.f) {
		state->fps = state->frames_count;
		state->frames_count = 0;
		state->timer_frame = 0;
	}

	for (int i = 0; i < state->player_count; i++) {
		if ((int)state->player[i].entity->pos.x <= 0 ||
			(int)state->player[i].entity->pos.y <= 0 ||
			(int)(state->player[i].entity->pos.x
			       + state->player[i].entity->size.x) >= 511 ||
			(int)(state->player[i].entity->pos.y
			       + state->player[i].entity->size.x) >= 481)
			state->player[i].speed = 0;

		/* IO */
		if (RPI_GetGpioValue(state->player[i].right_pin) == 0) {
			state->player[i].speed = 50;
			state->player[i].angular_vel = 270;
		} else if (RPI_GetGpioValue(state->player[i].left_pin) == 0) {
			state->player[i].speed = 50;
			state->player[i].angular_vel = -270;
		} else
			state->player[i].angular_vel = 0;
	}


	/* Boxes */
	/* Increase number of boxes over time */
	if (state->timer_box > BOX_TIMER
				&& state->boxes_count < BOX_COUNT_MAX) {
		state->timer_box = 0;
		add_box(state);
	}
	/* Move Boxes */
	for (int i = 0; i < state->boxes_count; i++)
		move_box(state, &state->boxes[i]);

	/* Player */
	for (int i = 0; i < state->player_count; i++) {
		if (state->player[i].lives > 0)
			move_player(state, &state->player[i]);
	}

}

void game_draw(game_state_t *state)
{
	/* background */
	draw_background(state);

	/* boxes */
	for (int i = 0; i < state->boxes_count; i++)
		draw_box(state, &state->boxes[i]);


	/* player */
	for (int i = 0; i < state->player_count; i++)
		draw_player(state, &state->player[i]);

	/* UI */
	print_time(state);
	print_lives(state);
	print_io(state);
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

void game_over(game_state_t *state)
{

	char str[16];

	draw_background(state);
	print_text(state, "GAME OVER", &(vector2_t){165, 181});

	if (state->player_count == 1) {
		sprintf(str, "TIME: %.1f", state->timer_game);
		print_text(state, str, &(vector2_t){165, 231});
	} else {
		for (int i = 0; i < state->player_count; i++)
			if (state->player[i].lives > 0)
				sprintf(str, "PLAYER %i WON", i + 1);

		print_text(&state, str, &(vector2_t){136, 231});
	}

}
