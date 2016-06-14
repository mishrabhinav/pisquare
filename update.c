#include "update.h"

#include <math.h>

#include "random.h"

void update_entity(game_state_t *state, entity_t *ent)
{
	float delta = state->delta;

	ent->pos.x += delta * ent->vel.x;
	ent->pos.y += delta * ent->vel.y;
}

void update_box(game_state_t *state, box_t *box)
{
	entity_t *entity = box->entity;

	update_entity(state, entity);

	if ((entity->pos.x > state->area.x)
		|| (entity->pos.x + entity->size.x  < 0)
		|| (entity->pos.y > state->area.y)
		|| (entity->pos.y + entity->size.y < 0)) {
		regenerate_box(state, box);
	}
}

void regenerate_box(game_state_t *state, box_t *box)
{
	int up = random_int(2);
	int forward = random_int(2);
	int sign = 2 * forward - 1;
	float vel = sign * (random_int(50) + 10);

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
	int rand_size = 6 + random_int(7);
	int grey = random_int(128) + 128;

	box->entity->size = (vector2_t){rand_size, rand_size};
	box->entity->pos = (vector2_t){posx, posy};
	box->entity->vel = (vector2_t){!up * vel, up * vel};
	box->color = (color_t){grey, grey, grey, 255};
}

void update_player(game_state_t *state, player_t *player)
{
	/* Timing */
	player->timer_flash += state->delta;
	if (player->timer_flash > PLAYER_TIMER_FLASH)
		player->timer_flash = 0;

	/* Movement */
	player->debounce_time += state->delta;
	player->dir = fmodf(player->dir + player->angular_vel * state->delta,
									360);
	player->entity->vel.x = player->speed * cos(M_PI * player->dir/180.f);
	player->entity->vel.y = player->speed * sin(M_PI * player->dir/180.f);

	update_entity(state, player->entity);

	/* Speed Increase */
	if (state->time > MAX_DIFFICULTY_TIME)
		player->speed = PLAYER_SPEED_INCREASE;
	else
		player->speed = PLAYER_DEFAULT_SPEED
			+ PLAYER_SPEED_INCREASE
			* state->time / MAX_DIFFICULTY_TIME;

	/* Boundaries */
	if ((int)player->entity->pos.x <= 0 ||
		(int)player->entity->pos.y <= 0 ||
		(int)(player->entity->pos.x
		       + player->entity->size.x) >= 511 ||
		(int)(player->entity->pos.y
		       + player->entity->size.x) >= 481)
		player->speed = 0;

	/* IO */
	if (RPI_GetGpioValue(player->right_pin) == 0)
		player->angular_vel = 270;
	else if (RPI_GetGpioValue(player->left_pin) == 0)
		player->angular_vel = -270;
	else
		player->angular_vel = 0;
}
