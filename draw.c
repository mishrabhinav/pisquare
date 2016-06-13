#include "draw.h"

#include <math.h>

#include "renderer.h"
#include "assets/assets.h"

void draw_splash(const game_state_t *state)
{
	graphics_draw_image(state->device, &(vector2_t){0, 0},
			    splash_bin, NULL);

	cvertex_t tr[3] = {
		[0] = {
			.pos = { .x = 0, .y = 0 },
			.color = { .r = 255, .g = 0, .b = 0, .a = 255 }
		},
		[1] = {
			.pos = { .x = 0, .y = 200 },
			.color = { .r = 0, .g = 255, .b = 0, .a = 255 }
		},
		[2] = {
			.pos = { .x = 200, .y = 0 },
			.color = { .r = 0, .g = 0, .b = 255, .a = 255 }
		},
	};

	graphics_draw(state->device, tr, 3);

	graphics_flush(state->device);
}

void draw_background(const game_state_t *state)
{
	graphics_draw_image(state->device, &(vector2_t){0, 0},
			    background_bin, NULL);
}

void draw_rect(const game_state_t *state, const rect_t *rect,
	       const color_t *col)
{
	graphics_draw_rectangle(state->device, col,
				&rect->origin, &rect->size);
}

void draw_square(const game_state_t *state, vector2_t *pos, float width,
					float degrees, const color_t *color) {
	cvertex_t vs[6];
	float s = sinf((float)M_PI * degrees/180.f);
	float c = cosf((float)M_PI * degrees/180.f);

	vs[0].pos = (vector2_t){pos->x + (c - s) * width/2,
					pos->y + (s + c) * width/2};
	vs[1].pos = (vector2_t){pos->x - (c + s) * width/2,
					pos->y + (c - s) * width/2};
	vs[2].pos = (vector2_t){pos->x + (s + c) * width/2,
						pos->y + (s - c) * width/2};
	vs[3] = vs[2];
	vs[4] = vs[1];
	vs[5].pos = (vector2_t){pos->x + (s - c) * width/2,
					pos->y - (s + c) * width/2};


	vs[0].color = *color;
	vs[1].color = *color;
	vs[2].color = *color;
	vs[3].color = *color;
	vs[4].color = *color;
	vs[5].color = *color;

	graphics_draw(state->device, vs, 6);
}

void draw_box(const game_state_t *state, const box_t *box)
{
	rect_t rect;

	draw_rect(state, entity_rect(&rect, box->entity), &box->color);
}

void draw_player(const game_state_t *state, const player_t *player)
{
	/* rect_t rect; */

	/* draw_rect(state, entity_rect(&rect, player->entity),
	 *						player->color);
	*/
	/* generate vertices */
	 cvertex_t vs[3];
	float s = sinf((float)M_PI * player->dir/180.f);
	float c = cosf((float)M_PI * player->dir/180.f);
	vector2_t pos = player->entity->pos;
	vector2_t size = player->entity->size;

	vs[0].pos = (vector2_t){pos.x + c * size.x/2, pos.y + s * size.y/2};
	vs[1].pos = (vector2_t){pos.x - (c + s) * size.x/2,
						pos.y + (c - s) * size.y/2};
	vs[2].pos = (vector2_t){pos.x + (s - c) * size.x/2,
						pos.y - (s + c) * size.y/2};

	vs[0].color = player->color;
	vs[1].color = player->color;
	vs[2].color = player->color;

	graphics_draw(state->device, vs, 3);

	/*draw_square(state, &player->entity->pos, player->entity->size.x,
	*		player->dir, &player->color);
	*/
}
