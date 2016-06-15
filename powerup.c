#include "powerup.h"

#include <stdlib.h>
#include "random.h"

powerup_t *powerup_new(powerup_t *powerup)
{
	powerup->color = (color_t){.r = 255, .g = 255, .b = 0, .a = 255};
	powerup->entity = entity_new();
	powerup->type = POWERUP_TYPE_LIFE;
	powerup->entity->size = (vector2_t){20, 20};
	powerup->free = 0;

	return powerup;
}

void powerup_set_kind(powerup_t *powerup, int type)
{
	powerup->type = type;
}

void powerup_free(powerup_t *powerup)
{
	entity_free(powerup->entity);
	free(powerup);
}
