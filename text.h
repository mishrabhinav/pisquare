#ifndef TEXT_H
#define TEXT_H

#include "gamestate.h"
#include "typedefs.h"

void print_text(const game_state_t *state,
		const char *string, const vector2_t *pos);
void print_text_color(const game_state_t *state, const char *string,
		      const vector2_t *pos, const color_t *col);
unsigned char *get_letter(char letter);

#endif
