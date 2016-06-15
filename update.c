#include "update.h"

#include <math.h>

#include "random.h"
#include "color.h"

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

void update_bullet(game_state_t *state, bullet_t *bullet)
{
	entity_t *entity = bullet->entity;

	update_entity(state, entity);

	if ((entity->pos.x > state->area.x)
		|| (entity->pos.x + entity->size.x  < 0)
		|| (entity->pos.y > state->area.y)
		|| (entity->pos.y + entity->size.y < 0)) {
		bullet->dead = 1;
	}
}


void regenerate_box(game_state_t *state, box_t *box)
{
	int up = random_int(2);
	int forward = random_int(2);
	int sign = 2 * forward - 1;
	float vel = sign * (random_int(BOX_SPEED_VARIANCE_DEFAULT
			+ state->difficulty * BOX_SPEED_VARIANCE_INCREASE)
			+ BOX_SPEED_DEFAULT
			+ state->difficulty * BOX_SPEED_INCREASE);

	int randy = random_int(state->area.y - box->entity->size.y);
	int randx = random_int(state->area.x - box->entity->size.x);

	int posx = !up * (-box->entity->size.x + !forward * (state->area.x
						+ box->entity->size.x))
						+ up * randx;
	int posy = up * (-box->entity->size.y + !forward * (state->area.y
						+ box->entity->size.y))
						+ !up * randy;
	int rand_size = BOX_SIZE_DEFAULT + random_int(BOX_SIZE_VARIANCE);
	int grey = random_int(128) + 128;

	box->entity->size = (vector2_t){rand_size, rand_size};
	box->entity->pos = (vector2_t){posx, posy};
	box->entity->vel = (vector2_t){!up * vel, up * vel};
	box->color = (color_t){grey, grey, grey, 255};
}

static inline float min(float a, float b)
{
	return a < b ? a : b;
}

void update_player(game_state_t *state, player_t *player)
{
	/* Timing */
	player->timer_flash += state->delta;
	player->timer_shoot += state->delta;
	player->timer_powerup_bullets += state->delta;
	player->timer_powerup_bullets_delay += state->delta;
	player->timer_powerup_ghost += state->delta;
	player->timer_powerup_tiny += state->delta;

	/* Low-Life Flashing */
	if (player->timer_flash > PLAYER_TIMER_FLASH)
		player->timer_flash = 0.f;

	/* Movement */
	player->debounce_time += state->delta;

	player_rotate(player, player->angular_vel * state->delta);
	vector2_t dir = player_direction_vector(player);

	player->entity->vel.x = player->speed * dir.x;
	player->entity->vel.y = player->speed * dir.y;

	update_entity(state, player->entity);

	/* Speed Increase */
	player->speed = PLAYER_SPEED_DEFAULT
				+ state->difficulty * PLAYER_SPEED_INCREASE;

	/* Boundaries */
	if (player->entity->pos.x <= 0) { /* left wall */
		player->entity->vel.x = -player->entity->vel.x;
		player->entity->pos.x = 0;
	} else if (player->entity->pos.x /* right wall */
				+ player->entity->size.x >= state->area.x) {
		player->entity->vel.x = -player->entity->vel.x;
		player->entity->pos.x =
				state->area.x - player->entity->size.x - 1;
	}

	if (player->entity->pos.y <= 0) { /* top wall */
		player->entity->vel.y = -player->entity->vel.y;
		player->entity->pos.y = 0;
	} else if (player->entity->pos.y  /* bottom wall */
				+ player->entity->size.y >= state->area.y) {
		player->entity->vel.y = -player->entity->vel.y;
		player->entity->pos.y =
				state->area.y - player->entity->size.y - 1;
	}

	/* Correct Direction */
	player->dir = 180
		* atan2(player->entity->vel.y, player->entity->vel.x)/M_PI;

	/* Motion Trail */
	/* Shift and recolor */
	if (player->trail_toggle) {
		for (int i = PLAYER_TRAIL_SEGMENTS-1; i > 0; i--)
			player->trail[i] = player->trail[i-1];
		player->trail[0] = (cvertex_t){{player->entity->pos.x
						+ player->entity->size.x/2,
						player->entity->pos.y
						+ player->entity->size.y/2},
								player->color};
	}
	player->trail_toggle = !player->trail_toggle;

	/* Powerup State */
	if (player->powerup_bullets
		&& player->timer_powerup_bullets > POWERUP_BULLETS_DURATION)
		player->powerup_bullets = 0;
	if (player->powerup_tiny
		&& player->timer_powerup_tiny > POWERUP_TINY_DURATION)
		player->powerup_tiny = 0;
	if (player->powerup_ghost && player->timer_powerup_ghost
						> POWERUP_GHOST_DURATION)
		player->powerup_ghost = 0;

	/* Powerup Effects */
	/* Bullets */
	if (player->powerup_bullets
		&& player->timer_powerup_bullets_delay
		> POWERUP_BULLETS_DELAY) {
		player->shoot = 1;
		player->timer_powerup_bullets_delay = 0.f;
	}
	/* Sizing */
	if (player->powerup_tiny)
		player_resize(player, PLAYER_SIZE_TINY);
	else
		player_resize(player, PLAYER_SIZE_DEFAULT);

	/* State Update */
	player->normal = player->debounce_time > PLAYER_DEBOUNCE_TIME;

	/* IO */
	int right = !RPI_GetGpioValue(player->right_pin);
	int left = !RPI_GetGpioValue(player->left_pin);

	if (right && !left)
		player->angular_vel = PLAYER_SPEED_ANGULAR;
	else if (!right && left)
		player->angular_vel = -PLAYER_SPEED_ANGULAR;
	else
		player->angular_vel = 0;
	if (right && left)
		if (player->timer_shoot > PLAYER_TIMER_SHOOT)
			player->shoot = 1;
}
