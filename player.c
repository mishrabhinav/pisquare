#include "player.h"

#include <stdlib.h>
#include <math.h>

static float player_number_dir(int player, int player_count)
{
	if (player_count == 1)
		return PLAYER_DIRECTION_DEFAULT;
	else if (player_count > 2)
		return 315 - 90 * (player - 1);
	else
		return 315 - 180 * (player - 1);
}

player_t *player_new(int num, int player_count)
{
	player_t *new = malloc(sizeof(player_t));

	new->entity = entity_new();
	new->entity->size = (vector2_t){PLAYER_SIZE_DEFAULT,
							PLAYER_SIZE_DEFAULT};
	new->lives = PLAYER_LIVES_DEFAULT;
	new->dir = player_number_dir(num, player_count);
	new->speed = PLAYER_SPEED_DEFAULT;
	new->debounce_time = PLAYER_DEBOUNCE_TIME;

	new->entity->pos = (vector2_t){0, 0};
	new->angular_vel = 0;

	/* State Flags */
	new->normal = 1;
	new->shoot = 0;
	new->powered = 0;

	/* Powerup Flags */
	new->powerup_tiny = 0;
	new->powerup_bullets = 0;
	new->powerup_invincible = 0;
	new->powerup_shield = 0;

	/* Timers */
	new->timer_flash = 0.f;
	new->timer_shoot = 0.f;
	new->timer_powerup_bullets = 0.f;
	new->timer_powerup_invincible = 0.f;
	new->timer_powerup_tiny = 0.f;

	/* Color */
	new->color = player_number_color(num);

	/* Input Pin Assignment */
	new->right_pin = player_number_right_pin(num);
	new->left_pin = player_number_left_pin(num);

	return new;
}

void player_rotate(player_t *player, float angle)
{
	player->dir = fmodf(player->dir + angle, 360);
}

void player_injure(player_t *player)
{
	--player->lives;
	player->debounce_time = 0;
}

void player_shoot(player_t *player, bullet_t *bullet)
{
	vector2_t dir = player_direction_vector(player);

	bullet->entity->pos = (vector2_t){player->entity->pos.x
				+ player->entity->size.x/2 - 2 + 15 * dir.x,
					player->entity->pos.y
				+ player->entity->size.y/2 - 2 + 15 * dir.y};

	bullet->entity->vel = (vector2_t){4 * player->entity->vel.x,
						4 * player->entity->vel.y};
	bullet->dead = 0;
	player->timer_shoot = 0;
	player->shoot = 0;
}

void player_powerup(player_t *player, powerup_t *powerup)
{
	powerup->free = 1;

	switch (powerup->type) {
	case POWERUP_TYPE_BULLETS:
		player->powerup_bullets = 1;
		player->timer_powerup_bullets = 0.f;
		break;
	case POWERUP_TYPE_LIFE:
		player->lives = player->lives == PLAYER_LIVES_MAX
			? PLAYER_LIVES_MAX : player->lives + 1;
		break;
	case POWERUP_TYPE_SHIELD:
		player->powerup_shield = 1;
		break;
	case POWERUP_TYPE_TINY:
		player->powerup_tiny = 1;
		player->timer_powerup_tiny = 0.f;
		break;
	case POWERUP_TYPE_INVINCIBLE:
		player->powerup_invincible = 1;
		player->timer_powerup_invincible = 0.f;
		break;
	}
}

vector2_t player_direction_vector(const player_t *player)
{
	return (vector2_t){cosf((float)M_PI * player->dir/180.f),
					sinf((float)M_PI * player->dir/180.f)};
}

rpi_gpio_pin_t player_number_left_pin(int player)
{
	switch (player) {
	case 1:
		return PLAYER_1_LEFT;
	case 2:
		return PLAYER_2_LEFT;
	case 3:
		return PLAYER_3_LEFT;
	case 4:
		return PLAYER_4_LEFT;
	default:
		return PLAYER_1_LEFT;
	}
}

rpi_gpio_pin_t player_number_right_pin(int player)
{
	switch (player) {
	case 1:
		return PLAYER_1_RIGHT;
	case 2:
		return PLAYER_2_RIGHT;
	case 3:
		return PLAYER_3_RIGHT;
	case 4:
		return PLAYER_4_RIGHT;
	default:
		return PLAYER_1_RIGHT;
	}
}


color_t player_number_color(int player)
{
	switch (player) {
	case 1:
		return PLAYER_1_COLOR;
	case 2:
		return PLAYER_2_COLOR;
	case 3:
		return PLAYER_3_COLOR;
	case 4:
		return PLAYER_4_COLOR;
	default:
		return PLAYER_1_COLOR;
	}
}

void player_free(player_t *player)
{
	entity_free(player->entity);
	free(player);
}
