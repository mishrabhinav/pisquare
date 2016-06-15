#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rpi-main.h"
#include "rpi-armtimer.h"
#include "rpi-gpio.h"
#include "rpi-interrupts.h"
#include "rpi-mailbox-interface.h"
#include "rpi-systimer.h"

#include "splash.h"
#include "menu.h"
#include "menu_players.h"
#include "game.h"
#include "game_over.h"
#include "io.h"

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
static scene_t *scene, *splash, *menu, *menu_players, *game, *game_over;

static void goto_scene(scene_t *next)
{
	/*if (next && next->free)
	*	next->free(&state);
	*/
	io_reset(&state);
	scene = next;
	scene->init(&state);
	state.time = 0;
}

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

	/* set main controller */
	RPI_WaitMicroSeconds(1500000);

	RPI_SetGpioInput(PLAYER_1_RIGHT);
	RPI_GetGpio()->GPPUD = 2;
	RPI_SetGpioInput(PLAYER_1_LEFT);
	RPI_GetGpio()->GPPUD = 2;

	/* prepare scenes */
	splash = splash_scene();
	menu = menu_scene();
	menu_players = menu_players_scene();
	game = game_scene();
	game_over = game_over_scene();

	splash->next_scene = menu;
	menu->next_scene = menu_players;
	menu_players->next_scene = game;
	game->next_scene = game_over;
	game_over->next_scene = menu;

	/* state configuration */
	state.player_count = 1;

	/* start at splash */
	goto_scene(splash);

	/* configure time tracking */
	rpi_sys_timer_t *timer = RPI_GetSystemTimer();
	uint32_t prev = timer->counter_lo;
	uint32_t new;
	uint8_t proceed;

	/* main loop */
	while (1) {
		/* time delta */
		new = timer->counter_lo;
		state.delta = (float)(new - prev)/1000000;
		prev = new;
		state.time += state.delta;

		/* IO */
		io_update(&state);

		/* update */
		proceed = scene->update(&state);

		/* perform scene transitions */
		if (!proceed)
			goto_scene(scene->next_scene);
		/* draw */
		scene->draw(&state);

		/* write full frame */
		graphics_flush(state.device);
	}

	draw_background(&state);
	graphics_flush(state.device);
}
