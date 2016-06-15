#include "powerup.h"

#include <stdlib.h>
#include "random.h"

powerup_t *powerup_new(powerup_t *powerup)
{
	powerup->color = (color_t){.r = 255, .g = 255, .b = 0, .a = 255};
	powerup->entity = entity_new();
	powerup->type = POWERUP_TYPE_BULLETS;
	powerup->free = 0;
	powerup->duration = 0.f;

	return powerup;
}
static inline float powerup_duration(int type)
{
	switch (type) {
	case POWERUP_TYPE_BULLETS:
		return POWERUP_BULLETS_DURATION;
	case POWERUP_TYPE_LIFE:
		return POWERUP_LIFE_DURATION;
	case POWERUP_TYPE_SHIELD:
		return POWERUP_SHIELD_DURATION;
	case POWERUP_TYPE_TINY:
		return POWERUP_TINY_DURATION;
	case POWERUP_TYPE_INVINCIBLE:
		return POWERUP_INVINCIBLE_DURATION;
	default:
		return 0;
	}
}

void powerup_set_kind(powerup_t *powerup, int type)
{
	powerup->type = type;
	powerup->duration = powerup_duration(type);
}

void powerup_free(powerup_t *powerup)
{
	entity_free(powerup->entity);
	free(powerup);
}
