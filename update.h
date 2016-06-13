#ifndef UPDATE
#define UPDATE

#include "gamestate.h"
#include "entity.h"
#include "box.h"
#include "sprite.h"
#include "player.h"

void update_entity(game_state_t *state, entity_t *ent);
void update_box(game_state_t *state, box_t *box);
void update_player(game_state_t *state, player_t *player);
void update_sprite(game_state_t *state, sprite_t *player);

#endif
