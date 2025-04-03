#include "common.h"

#include "cglm/struct/cam.h"
#include "sokol_time.h"

typedef enum {
    PROJ_PERSPECTIVE,
    PROJ_ORTHOGRAPHIC,
} projection_e;

static struct {
    vec3s position;
    vec3s target;
    vec3s world_up;

    projection_e projection;
    mat4s view;
    mat4s persp_proj;
    mat4s ortho_proj;
} camera;

float camera_aspect_impl(void);

void camera_init(void) {
    camera.position = (vec3s) { { 0.0f, 0.0f, -3.0f } };
    camera.target = (vec3s) { { 0.0f, 0.0f, 0.0f } };
    camera.world_up = (vec3s) { { 0.0f, -1.0f, 0.0f } }; // Intentionally inverted to flip screen.

    camera.view = glms_lookat(camera.position, camera.target, camera.world_up);
    camera.persp_proj = glms_perspective(45.0f, camera_aspect_impl(), 0.1f, 100.0f);
    camera.ortho_proj = glms_ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
    camera.projection = PROJ_PERSPECTIVE;
}

void camera_toggle(void) {
    camera.projection = (camera.projection == PROJ_PERSPECTIVE) ? PROJ_ORTHOGRAPHIC : PROJ_PERSPECTIVE;
}

mat4s camera_proj(void) {
    return camera.projection == PROJ_PERSPECTIVE ? camera.persp_proj : camera.ortho_proj;
}

mat4s camera_view(void) {
    return camera.view;
}

static struct {
    uint64_t last_time;
    float fps;
    int frame_count;
    float elapsed_sec;
} clock;

void clock_init(void) {
    stm_setup();
}

void clock_frame(void) {
    uint64_t now = stm_now();
    float dt = (float)stm_sec(stm_diff(now, clock.last_time));
    clock.last_time = now;

    clock.elapsed_sec += dt;
    clock.frame_count++;

    if (clock.elapsed_sec >= 1.0f) {
        clock.fps = (float)clock.frame_count / clock.elapsed_sec;
        clock.frame_count = 0;
        clock.elapsed_sec = 0.0f;
    }
}

float clock_fps(void) {
    return clock.fps;
}
