#ifndef POWERUP
#define POWERUP

#include "entity.h"
#include "color.h"

#define POWERUP_BULLETS_COLOR (color_t){0, 255, 255, 255}
#define POWERUP_BULLETS_DURATION 3
#define POWERUP_BULLETS_DELAY 0.12f

#define POWERUP_LIFE_COLOR (color_t){0, 0, 255, 255}
#define POWERUP_LIFE_DURATION 0

#define POWERUP_SHIELD_COLOR (color_t){255, 255, 255, 255}
#define POWERUP_SHIELD_DURATION 0

#define POWERUP_TINY_COLOR (color_t){0, 255, 0, 255}
#define POWERUP_TINY_DURATION 6

#define POWERUP_GHOST_COLOR (color_t){0, 0, 0, 255}
#define POWERUP_GHOST_DURATION 5

enum {
	POWERUP_TYPE_BULLETS,
	POWERUP_TYPE_LIFE,
	POWERUP_TYPE_SHIELD,
	POWERUP_TYPE_TINY,
	POWERUP_TYPE_GHOST,
	POWERUP_TYPE_NONE
};

typedef struct {
	entity_t *entity;
	color_t color;
	int type;
	int free;
} powerup_t;

powerup_t *powerup_new(powerup_t *powerup);
void powerup_set_type(powerup_t *powerup, int type);
void powerup_free(powerup_t *powerup);

#endif
