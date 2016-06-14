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

	if (entity->pos.x > state->area.x) {
		entity->pos.x = -entity->size.x;
		entity->pos.y = random_int(state->area.y
						-entity->size.y)
						+ entity->size.y/2;
	} else if (entity->pos.x + entity->size.x  < 0) {
		entity->pos.x = state->area.x;
		entity->pos.y = random_int(state->area.y
						-entity->size.y)
						+ entity->size.y/2;
	}
	if (entity->pos.y > state->area.y) {
		entity->pos.y = -entity->size.y;
		entity->pos.x = random_int(state->area.x
						-entity->size.x)
						+ entity->size.x/2;
	} else if (entity->pos.y + entity->size.y < 0) {
		entity->pos.y = state->area.y;
		entity->pos.x = random_int(state->area.x
						-entity->size.x)
						+ entity->size.x/2;
	}

	update_entity(state, entity);
}

void update_player(game_state_t *state, player_t *player)
{
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
