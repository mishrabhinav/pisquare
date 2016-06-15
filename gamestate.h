#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "renderer.h"
#include "player.h"
#include "box.h"
#include "bullet.h"
#include "powerup.h"

#define BOX_SPAWN_TIMER 1.0f
#define BOX_COUNT_MAX 50

#define POWERUP_SPAWN_TIME_DEFAULT 2
#define POWERUP_SPAWN_TIME_VARIANCE 3

#define POWERUP_COUNT_MAX 10

#define BOX_SPEED_DEFAULT 50
#define BOX_SPEED_INCREASE 100
#define BOX_SPEED_VARIANCE_DEFAULT 10
#define BOX_SPEED_VARIANCE_INCREASE 40

#define BULLET_COUNT_MAX 100

#define MAX_DIFFICULTY_TIME 120.0f

typedef struct game_state {
	graphics_t *device;

	vector2_t area;

	float delta;

	player_t *player;

	box_t *boxes;
	bullet_t *bullets;
	powerup_t *powerups;

	int boxes_count;
	int bullets_count;
	int powerups_count;
	int player_count;
	int frames_count;
	int fps;

	float difficulty; /* increases with time */

	/* Scene & Game Timers */
	float time;
	float timer_frame;
	float timer_box;
	float timer_game;
	float timer_powerup;
	float powerup_wait; /* time before next power-up spawns */

	/* IO */
	int p1_left_down;
	int p1_right_down;
	int p1_left_clicked;
	int p1_right_clicked;
	int p1_left_held;
	int p1_right_held;
	int p1_left_released;
	int p1_right_released;
	float p1_left_debounce_timer;
	float p1_right_debounce_timer;
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
