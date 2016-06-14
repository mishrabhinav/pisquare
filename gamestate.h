#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "renderer.h"
#include "player.h"
#include "box.h"

#define BOX_TIMER 1.0f
#define BOX_COUNT_MAX 50

#define MAX_DIFFICULTY_TIME 120.0f

typedef struct game_state {
	graphics_t *device;

	vector2_t area;

	float delta;

	player_t *player;

	box_t *boxes;

	int boxes_count;
	int player_count;
	int frames_count;
	int fps;

	/* Timers */
	float time;
	float timer_frame;
	float timer_box;
	float timer_game;
} game_state_t;

typedef struct scene {
	/* Functions */
	void (*init)(game_state_t *state);
	int (*update)(game_state_t *state);
	void (*draw)(game_state_t *state);
	void (*free)(game_state_t *state);

	/* Transition */
	struct scene *next_scene;
} scene_t;

#endif
