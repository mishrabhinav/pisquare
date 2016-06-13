#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rpi-main.h"
#include "rpi-armtimer.h"
#include "rpi-gpio.h"
#include "rpi-interrupts.h"
#include "rpi-mailbox-interface.h"
#include "rpi-systimer.h"

#include "game.h"
#include "text.h"
#include "renderer.h"
#include "dma.h"
#include "draw.h"
#include "player.h"

#define SCREEN_WIDTH    512
#define SCREEN_HEIGHT	512
#define SCREEN_DEPTH    32      /* 16 or 32-bit */
#define MAX_PLAYERS	4

static game_state_t state;

void kernel_main(unsigned int r0, unsigned int r1, unsigned int atags)
{
	rpi_mailbox_property_t *mp;

	(void)r0;
	(void)r1;
	(void)atags;

	/* Write 1 to the LED init nibble in the Function Select GPIO
	 * peripheral register to enable LED pin as an output
	 */
	RPI_GetGpio()->LED_GPFSEL |= LED_GPFBIT;

	/* Enable the timer interrupt IRQ */
	RPI_GetIrqController()->Enable_Basic_IRQs = RPI_BASIC_ARM_TIMER_IRQ;

	/* Setup the system timer interrupt */
	/* Timer frequency = Clk/256 * 0x400 */
	RPI_GetArmTimer()->Load = 0x400;

	/* Setup the ARM Timer */
	RPI_GetArmTimer()->Control =
		RPI_ARMTIMER_CTRL_23BIT |
		RPI_ARMTIMER_CTRL_ENABLE |
		RPI_ARMTIMER_CTRL_INT_ENABLE |
		RPI_ARMTIMER_CTRL_PRESCALE_256;

	/* Enable interrupts! */
	_enable_interrupts();

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_BOARD_MODEL);
	RPI_PropertyProcess();

	mp = RPI_PropertyGet(TAG_GET_BOARD_MODEL);

	/* Ensure the ARM is running at it's maximum rate */
	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_SET_CLOCK_RATE, TAG_CLOCK_ARM,
					mp->data.buffer_32[1]);
	RPI_PropertyProcess();

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_CLOCK_RATE, TAG_CLOCK_ARM);
	RPI_PropertyProcess();

	/* Initialize DMA */
	dma_init();

	state.device = graphics_create(SCREEN_WIDTH,
				       SCREEN_HEIGHT,
				       SCREEN_DEPTH);

	state.player = calloc(MAX_PLAYERS + 1, sizeof(player_t));

	draw_splash(&state);

	RPI_WaitMicroSeconds(800000);

	/* set main controller */
	RPI_SetGpioInput(PLAYER_1_RIGHT);
	RPI_GetGpio()->GPPUD = 2;
	RPI_SetGpioInput(PLAYER_1_LEFT);
	RPI_GetGpio()->GPPUD = 2;

menu:
	/* show menu */
	game_menu(&state);

	/* initialize round */
	game_init(&state);

	int alive;

	rpi_sys_timer_t *timer = RPI_GetSystemTimer();
	uint32_t prev = timer->counter_lo;
	uint32_t new;

	/* main loop */
	while (1) {
		alive = state.player_count;

		/* time delta */
		new = timer->counter_lo;
		state.delta = (float)(new - prev)/1000000;
		prev = new;

		/* update */
		game_update(&state);

		/* draw */
		game_draw(&state);

		/* write full frame */
		graphics_flush(state.device);

		/* check for alive players */
		for (int i = 0; i < state.player_count; i++)
			if (state.player[i].lives == 0)
				alive--;

		if ((state.player_count == 1 && !alive) ||
			(state.player_count > 1 && alive == 1))
			break;
	}

	RPI_WaitMicroSeconds(500000);
	game_over(&state);
	print_text(&state, "PRESS RL TO RESTART", (vector2_t){66, 484});
	graphics_flush(state.device);

	while (RPI_GetGpioValue(PLAYER_1_RIGHT) != 0)
		;

	while (RPI_GetGpioValue(PLAYER_1_LEFT) != 0)
		;

	draw_background(&state);
	graphics_flush(state.device);

	goto menu;
}
