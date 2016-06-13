#include "player.h"

#include <stdlib.h>

player_t *player_new(void)
{
	player_t *new = malloc(sizeof(player_t));

	new->entity = entity_new();
	new->entity->size = (vector2_t){8, 8};
	new->lives = PLAYER_LIVES;
	new->dir = PLAYER_DIRECTION;
	new->speed = PLAYER_SPEED;
	new->color = (color_t){.r = 0, .g = 0, .b = 255, .a = 255};

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

color_t get_colour(int player)
{
	switch (player) {
	case 1:
		return PLAYER_1_COLOUR;
	case 2:
		return PLAYER_2_COLOUR;
	case 3:
		return PLAYER_3_COLOUR;
	case 4:
		return PLAYER_4_COLOUR;
	default:
		return PLAYER_1_COLOUR;
	}
}

void player_free(player_t *player)
{
	entity_free(player->entity);
	free(player);
}
