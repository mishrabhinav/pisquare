#include "bullet.h"

#include <stdlib.h>
#include "random.h"

bullet_t *bullet_new(void)
{
	bullet_t *bullet = malloc(sizeof(bullet_t));

	bullet->color = (color_t){.r = 255, .g = 255, .b = 0, .a = 255};
	bullet->entity = entity_new();
	bullet->entity->size = (vector2_t){4, 4};

	return bullet;
}

void bullet_free(bullet_t *bullet)
{
	entity_free(bullet->entity);
	free(bullet);
}
