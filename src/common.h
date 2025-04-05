#pragma once

#include <stdint.h>

#define FB_WIDTH   (256)
#define FB_HEIGHT  (240)
#define FB_SIZE    (FB_WIDTH * FB_HEIGHT)
#define WIN_WIDTH  (FB_WIDTH * 3)
#define WIN_HEIGHT (FB_HEIGHT * 3)

extern uint32_t color_black;
extern uint32_t color_white;
extern uint32_t color_red;
extern uint32_t color_green;
extern uint32_t color_blue;
extern uint32_t color_transparent;
