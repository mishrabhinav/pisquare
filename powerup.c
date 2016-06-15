#include "powerup.h"

#include <stdlib.h>
#include "random.h"

powerup_t *powerup_new(powerup_t *powerup)
{
	powerup->color = (color_t){.r = 255, .g = 255, .b = 0, .a = 255};
	powerup->entity = entity_new();
	powerup->type = POWERUP_TYPE_BULLETS;
	powerup->entity->size = (vector2_t){6, 6};
	powerup->free = 0;

	return powerup;
}

void powerup_set_type(powerup_t *powerup, int type)
{
	powerup->type = type;
	switch (type) {
	case POWERUP_TYPE_BULLETS:
		powerup->color = POWERUP_BULLETS_COLOR;
		break;
	case POWERUP_TYPE_LIFE:
		powerup->color = POWERUP_LIFE_COLOR;
		break;
	case POWERUP_TYPE_SHIELD:
		powerup->color = POWERUP_SHIELD_COLOR;
		break;
	case POWERUP_TYPE_TINY:
		powerup->color = POWERUP_TINY_COLOR;
		break;
	case POWERUP_TYPE_GHOST:
		powerup->color = POWERUP_GHOST_COLOR;
		break;
	}
}

void powerup_free(powerup_t *powerup)
{
	entity_free(powerup->entity);
	free(powerup);
}
