#include <stdio.h>

#include "cglm/struct/affine.h"
#include "cglm/struct/mat4.h"
#include "cglm/types-struct.h"

#include "sokol_app.h"
#include "sokol_log.h"
#include "sokol_time.h"

#include "camera.h"
#include "clock.h"
#include "common.h"

void gfx_init(void);
void gfx_frame(mat4s proj, mat4s view, mat4s model);
void gfx_cleanup(void);

static struct {
    vec3s translation;
    vec3s rotation;
    vec3s scale;
} state;

void init(void) {
    clock_init();
    camera_init();
    gfx_init();

    state.translation = (vec3s) { { 0.0f, 0.0f, 0.0f } };
    state.rotation = (vec3s) { { 0.0f, 0.0f, 0.0f } };
    state.scale = (vec3s) { { 1.0f, 1.0f, 1.0f } };
}

static mat4s model_matrix(void) {
    mat4s model = glms_mat4_identity();
    model = glms_scale(model, state.scale);
    model = glms_rotate_z(model, state.rotation.z);
    model = glms_rotate_y(model, state.rotation.y);
    model = glms_rotate_x(model, state.rotation.x);
    model = glms_translate(model, state.translation);
    return model;
}

void frame(void) {
    clock_update();

    state.rotation.y += 0.01f;

    gfx_frame(camera_proj(), camera_view(), model_matrix());

    static char title[64];
    snprintf(title, sizeof(title), "Engine - %.2f FPS", clock_fps());
    sapp_set_window_title(title);
}

void event(const sapp_event* event) {
    if (event->type == SAPP_EVENTTYPE_KEY_DOWN) {
        switch (event->key_code) {
        case SAPP_KEYCODE_ESCAPE:
            sapp_request_quit();
            break;
        case SAPP_KEYCODE_SPACE:
            camera_toggle();
            break;
        default:
            break;
        }
    }
}

void cleanup(void) {
    gfx_cleanup();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    sapp_desc desc = { 0 };
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = event;
    desc.width = WIN_WIDTH;
    desc.height = WIN_HEIGHT;
    desc.window_title = "Engine";
    desc.icon.sokol_default = true;
    desc.logger.func = slog_func;
    return desc;
}
