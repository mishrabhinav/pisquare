#include "splash.h"

#include "assets/assets.h"
#include "text.h"

scene_t *splash_scene(void)
{
	scene_t *s = malloc(sizeof(scene_t));

	s->init = &splash_init;
	s->update = &splash_update;
	s->draw = &splash_draw;

	return s;
}

void splash_init(game_state_t *state)
{
	(void)state;
}

int splash_update(game_state_t *state)
{
	return state->time < 3.0f;
}

void splash_draw(game_state_t *state)
{
	graphics_draw_image(state->device, &(vector2_t){0, 0},
			    splash_bin, NULL);
}
