/**
 * \file
 *
 * \brief Widget ToolKit training widget
 *
 * Copyright (C) 2014-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <stdio.h>

#include "app_widget.h"

/**
 * \brief Event command ID for the application widgets.
 *
 * \note The command IDs cannot be 0, since this value is reserved for no
 * command event callback for certain widgets.
 */
enum app_widget_ids {
	/** Event command ID for the slider. */
	SLIDER_ID = 1,
	/** Event command ID for the button. */
	BUTTON_ID,
};

/**
 * \name Color scheme
 *
 * @{
 */

/** Background color for application */
#define APP_BACKGROUND_COLOR        GFX_COLOR(50, 50, 50)

/** @} */

/**
 * \name Widget sizes and positions
 *
 * @{
 */

/** Label position on top of display */
#define LABEL_POS_X                 10
/** Label position on top of display */
#define LABEL_POS_Y                 10

/** Slider position */
#define SLIDER_POS_X                10
/** Slider position */
#define SLIDER_POS_Y                60
/** Slider size on display */
#define SLIDER_SIZE_X               80
/** Slider size on display */
#define SLIDER_SIZE_Y               40

/** Spacing from slider to progress bar */
#define SLIDER_PB_SPACING_X         10
/** Slider progress bar on display */
#define PB_SIZE_X                   SLIDER_SIZE_X
/** Slider progress bar on display */
#define PB_SIZE_Y                   SLIDER_SIZE_Y

/** @} */

/**
 * \name Widget configurations
 *
 * @{
 */

/** Max value for slider */
#define SLIDER_MAX_VALUE            100

/** @} */

/**
 * \name Static text strings
 *
 * @{
 */

/** Description for label */
const static char *demo_string = "Demonstrating widgets";

/** @} */

/**
 * \name Static variables
 *
 * @{
 */

/** Pointer to frame for application */
static struct wtk_basic_frame *main_frame;
/** Pointer to slider */
static struct wtk_slider *slider;
/** Pointer to progress bar */
static struct wtk_progress_bar *progress_bar;
/** Frame background bitmap */
static struct gfx_bitmap frame_background;
/** Counter for button */
static uint8_t counter;
/** Pointer to the sub-frame */
static struct wtk_basic_frame *sub_frame;
/** Sub-frame background bitmap */
static struct gfx_bitmap sub_frame_background;

/** @} */

/**
 * \brief Frame command events handler
 *
 * This function handles the command events generated by the widgets.
 *
 * \sa wtk_basic_frame_command_handler_t
 */
static bool widget_frame_command_handler(struct wtk_basic_frame *frame,
		win_command_t command_data)
{
	char command = (uintptr_t)command_data;

	switch (command) {
	case SLIDER_ID:
		wtk_progress_bar_set_value(progress_bar,
				wtk_slider_get_value(slider));
		break;

	case BUTTON_ID:
		/** \todo Add code here to handle button press. */
		break;
	}

	return false;
}

/**
 * \brief Frame draw event handler
 *
 * This function will draw the contents of the sub-frame.
 *
 * \sa wtk_basic_frame_draw_handler_t
 */
static void sub_frame_draw_handler(struct win_window *win,
		struct win_clip_region const *clip)
{
	char buffer[4];

	snprintf(buffer, sizeof(buffer), "%3d", counter);

	/**
	 * \todo Add code here to draw text on screen using the
	 * gfx_draw_string() function.
	 */
}

/**
 * \brief Setup widget demo
 *
 * This function launches the widget demo.
 */
void app_widget_launch()
{
	struct win_window *win_root;
	struct win_window *parent;
	struct win_area area;
	struct wtk_label *lbl;
	struct wtk_button *btn;

	/* Get pointer to root window */
	win_root = win_get_root();

	/* Application frame */

	/* Create a background bitmap using a solid color. */
	frame_background.type = GFX_BITMAP_SOLID;
	frame_background.data.color = APP_BACKGROUND_COLOR;

	/* Set the area to fill the entire screen */
	area.pos.x = 0;
	area.pos.y = 0;
	area.size.x = gfx_get_width();
	area.size.y = gfx_get_height();

	/*
	 * Create a basic frame with a specified background and command event
	 * handler. Check the return value if an error occurred while creating
	 * the widget.
	 */
	main_frame = wtk_basic_frame_create(win_root, &area,
			&frame_background, NULL,
			widget_frame_command_handler, NULL);
	if (!main_frame) {
		goto error_frame;
	}

	/* Get a pointer to the widget's window for adding sub-widgets. */
	parent = wtk_basic_frame_as_child(main_frame);

	/*
	 * Draw the frame by showing the frame widget's window. Any
	 * child-widgets and windows will not be shown before the parent
	 * widget/window is shown.
	 */
	win_show(parent);

	/* Application label */
	area.pos.x = LABEL_POS_X;
	area.pos.y = LABEL_POS_Y;
	/* Find an optimal size for the widget. */
	wtk_label_size_hint(&area.size, demo_string);

	/*
	 * Create the label and check the return value if an error occurred
	 * while creating the label.
	 */
	lbl = wtk_label_create(parent, &area, demo_string,
			GFX_COLOR(255, 255, 255), NULL, false);
	if (!lbl) {
		goto error_widget;
	}

	/* Draw the label by showing the label widget's window. */
	win_show(wtk_label_as_child(lbl));

	/* Application slider */
	area.pos.x = SLIDER_POS_X;
	area.pos.y = SLIDER_POS_Y;
	area.size.x = SLIDER_SIZE_X;
	area.size.y = SLIDER_SIZE_Y;

	/*
	 * Create the slider and check the return value if an error occurred
	 * while creating the slider.
	 */
	slider = wtk_slider_create(parent, &area, SLIDER_MAX_VALUE,
			SLIDER_MAX_VALUE / 2,
			WTK_SLIDER_HORIZONTAL | WTK_SLIDER_CMD_RELEASE,
			(win_command_t)SLIDER_ID);
	if (!slider) {
		goto error_widget;
	}

	/* Draw the slider by showing the slider widget's window. */
	win_show(wtk_slider_as_child(slider));

	/* Application progress bar, placed right of the slider. */
	area.pos.x += area.size.x + SLIDER_PB_SPACING_X;
	area.size.x = PB_SIZE_X;
	area.size.y = PB_SIZE_Y;

	/*
	 * Create the progress bar and check the return value if an error
	 * occurred while creating the progress bar.
	 */
	progress_bar = wtk_progress_bar_create(parent, &area, SLIDER_MAX_VALUE,
			SLIDER_MAX_VALUE / 2, GFX_COLOR(255, 255, 0),
			GFX_COLOR(90, 90, 90), WTK_PROGRESS_BAR_HORIZONTAL);
	if (!progress_bar) {
		goto error_widget;
	}

	/* Draw the progress bar by showing the progress bar widget's window. */
	win_show(wtk_progress_bar_as_child(progress_bar));

	/** \todo Add code to set up button here. */

	/** \todo Add code to set up basic frame here. */

	return;

error_widget:
	/* Destroy widget and all sub-widgets. */
	win_destroy(wtk_basic_frame_as_child(main_frame));
error_frame:
	/* Wait forever if an error occurred during setup. */
	while (1) {
	}
}