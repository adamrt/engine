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
#include "model.h"

void gfx_init(void);
void gfx_frame(mat4s proj, mat4s view, mat4s model);
void gfx_cleanup(void);

static struct {
    transform_t transform;
} state;

static void state_init(void) {
    state.transform = (transform_t) {
        .translation = { { 0.0f, 0.0f, 0.0f } },
        .rotation = { { 0.0f, 0.0f, 0.0f } },
        .scale = { { 1.0f, 1.0f, 1.0f } },
    };
}

static void init(void) {
    clock_init();
    camera_init();
    state_init();
    gfx_init();
}

static void frame(void) {
    clock_update();

    // Update scene
    state.transform.rotation.y += 0.01f;

    // Render frame
    mat4s proj = camera_proj();
    mat4s view = camera_view();
    mat4s model = model_matrix(state.transform);
    gfx_frame(proj, view, model);

    // Update FPS
    static char title[64];
    snprintf(title, sizeof(title), "Engine - %.2f FPS", clock_fps());
    sapp_set_window_title(title);
}

static void event(const sapp_event* event) {
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

static void cleanup(void) {
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
