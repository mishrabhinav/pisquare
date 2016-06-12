#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "color.h"
#include "libarmc/rpi-gpio.h"

#define PLAYER_LIVES 100
#define PLAYER_DIRECTION 90
#define PLAYER_SPEED 50

#define PLAYER_1_RIGHT RPI_GPIO4
#define PLAYER_1_LEFT RPI_GPIO7
#define PLAYER_1_POS ((vector2_t){226, 226})
#define PLAYER_1_COLOUR ((color_t){0, 255, 0, 255})

#define PLAYER_2_RIGHT RPI_GPIO2
#define PLAYER_2_LEFT RPI_GPIO3
#define PLAYER_2_POS ((vector2_t){286, 226})
#define PLAYER_2_COLOUR ((color_t){0, 0, 255, 255})

#define PLAYER_3_RIGHT RPI_GPIO10
#define PLAYER_3_LEFT RPI_GPIO9
#define PLAYER_3_POS ((vector2_t){286, 286})
#define PLAYER_3_COLOUR ((color_t){255, 0, 0, 255})

#define PLAYER_4_RIGHT RPI_GPIO8
#define PLAYER_4_LEFT RPI_GPIO7
#define PLAYER_4_POS ((vector2_t){226, 286})
#define PLAYER_4_COLOUR ((color_t){255, 255, 0, 255})

typedef struct {
	entity_t *entity;
	float dir;
	float speed;
	float angular_vel;
	int lives;
	color_t color;
	rpi_gpio_pin_t left_pin;
	rpi_gpio_pin_t right_pin;
	rpi_gpio_value_t left;
	rpi_gpio_value_t right;
} player_t;

player_t *player_new(void);
void player_free(player_t *player);
rpi_gpio_pin_t get_right_pin(int player);
rpi_gpio_pin_t get_left_pin(int player);
vector2_t get_pos(int player);
color_t get_colour(int player);

#endif
