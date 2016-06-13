#ifndef DRAW_H
#define DRAW_H

#include "gamestate.h"
#include "player.h"
#include "box.h"
#include "sprite.h"
#include "minmax.h"

/* Splash */
void draw_splash(const game_state_t *state);

/* Background */
void draw_background(const game_state_t *state);

/* Generic Drawing Functions */
void draw_rect(const game_state_t *state,
	       const rect_t *rect, const color_t *col);
void draw_sprite(const game_state_t *state, const sprite_t *sprite);

/* Box */
void draw_box(const game_state_t *state, const box_t *box);

/* Players */
void draw_player(const game_state_t *state, const player_t *player);

#endif
