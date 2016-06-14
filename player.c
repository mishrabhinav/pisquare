#include "player.h"

#include <stdlib.h>

player_t *player_new(void)
{
	player_t *new = malloc(sizeof(player_t));

	new->entity = entity_new();
	new->entity->size = (vector2_t){PLAYER_DEFAULT_SIZE,
							PLAYER_DEFAULT_SIZE};
	new->lives = PLAYER_DEFAULT_LIVES;
	new->dir = PLAYER_DEFAULT_DIRECTION;
	new->speed = PLAYER_DEFAULT_SPEED;
	new->debounce_time = PLAYER_DEBOUNCE_TIME;
	new->timer_flash = 0.f;
	new->color = (color_t){255, 255, 255, 255};

	return new;
}

rpi_gpio_pin_t get_left_pin(int player)
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

rpi_gpio_pin_t get_right_pin(int player)
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

vector2_t get_pos(int player)
{
	switch (player) {
	case 1:
		return PLAYER_1_POS;
	case 2:
		return PLAYER_2_POS;
	case 3:
		return PLAYER_3_POS;
	case 4:
		return PLAYER_4_POS;
	default:
		return PLAYER_1_POS;
	}
}

color_t get_color(int player)
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
