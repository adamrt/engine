#pragma once

#include <stdint.h>

#include "cglm/types-struct.h"

#define FB_WIDTH   (256 * 3)
#define FB_HEIGHT  (240 * 3)
#define FB_SIZE    (FB_WIDTH * FB_HEIGHT)
#define WIN_WIDTH  (FB_WIDTH)
#define WIN_HEIGHT (FB_HEIGHT)

typedef struct {
    vec3s position;
    vec4s color;
} vertex_t;

void camera_init(void);
void camera_toggle(void);
mat4s camera_proj(void);
mat4s camera_view(void);

void clock_init(void);
void clock_frame(void);
float clock_fps(void);
