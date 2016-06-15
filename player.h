#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "color.h"
#include "libarmc/rpi-gpio.h"
#include "bullet.h"
#include "powerup.h"

#define PLAYER_LIVES_DEFAULT 3
#define PLAYER_LIVES_MAX 3

#define PLAYER_SPEED_DEFAULT 80
#define PLAYER_SPEED_INCREASE 40
#define PLAYER_SPEED_MAX (PLAYER_SPEED_DEFAULT + PLAYER_SPEED_INCREASE)
#define PLAYER_SPEED_ANGULAR 300
#define PLAYER_SIZE_DEFAULT 12

#define PLAYER_DIRECTION_DEFAULT 90

#define PLAYER_DEBOUNCE_TIME 1.5f
#define PLAYER_TIMER_FLASH 0.3f
#define PLAYER_TIMER_SHOOT 1.0f
#define PLAYER_OFFSET_START 200

#define PLAYER_1_RIGHT RPI_GPIO4
#define PLAYER_1_LEFT RPI_GPIO7
#define PLAYER_1_COLOR ((color_t){0, 255, 0, 255})

#define PLAYER_2_RIGHT RPI_GPIO2
#define PLAYER_2_LEFT RPI_GPIO3
#define PLAYER_2_COLOR ((color_t){255, 0, 255, 255})

#define PLAYER_3_RIGHT RPI_GPIO10
#define PLAYER_3_LEFT RPI_GPIO9
#define PLAYER_3_COLOR ((color_t){255, 255, 0, 255})

#define PLAYER_4_RIGHT RPI_GPIO8
#define PLAYER_4_LEFT RPI_GPIO7
#define PLAYER_4_COLOR ((color_t){0, 150, 255, 255})

typedef struct {
	/* Motion */
	entity_t *entity;
	float dir; /* angle from x-axis in degrees */
	float speed;
	float angular_vel; /* angular velocity */


	/* Timing */
	float debounce_time; /* invincibility after collision */
	float timer_flash; /* counter for low-life flashing */
	float timer_shoot; /* reload */
	float timer_powerup_bullets;
	float timer_powerup_bullets_delay;
	float timer_powerup_ghost;
	float timer_powerup_tiny;

	/* State */
	int lives;

	/* Flags */
	int normal; /* can be damaged */
	int shoot; /* flag to shoot */

	int powerup_tiny;
	int powerup_bullets;
	int powerup_ghost;
	int powerup_shield;

	color_t color;

	/* IO */
	rpi_gpio_pin_t left_pin;
	rpi_gpio_pin_t right_pin;
} player_t;

player_t *player_new(int num, int player_count);
void player_rotate(player_t *player, float angle);
void player_injure(player_t *player);
void player_free(player_t *player);
void player_shoot(player_t *player, bullet_t *bullet);
void player_powerup(player_t *player, powerup_t *powerup);
vector2_t player_direction_vector(const player_t *player);
color_t player_number_color(int num);
rpi_gpio_pin_t player_number_right_pin(int player);

rpi_gpio_pin_t player_number_left_pin(int player);

#endif
