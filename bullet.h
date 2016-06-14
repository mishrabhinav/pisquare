#ifndef BULLET
#define BULLET

#include <stdbool.h>

#include "entity.h"
#include "color.h"

typedef struct {
	entity_t *entity;
	color_t color;
	bool dead;
} bullet_t;

bullet_t *bullet_new(bullet_t *bullet);
void bullet_free(bullet_t *);

#endif
