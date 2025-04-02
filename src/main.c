#include "cglm/struct/affine.h"
#include "cglm/struct/cam.h"
#include "cglm/struct/mat4.h"
#include "cglm/types-struct.h"

#include "sokol_app.h"
#include "sokol_log.h"

#include "common.h"

void init_impl(void);
void frame_impl(mat4s proj, mat4s view, mat4s model);
void cleanup_impl(void);
float aspect_impl(void);

void init(void) {
    init_impl();
}

void frame(void) {
    static vec3s translation = { { 0.0f, 0.0f, 0.0f } };
    static vec3s rotation = { { 0.0f, 0.0f, 0.0f } };
    static vec3s scale = { { 1.0f, 1.0f, 1.0f } };

    mat4s proj = glms_perspective(45.0f, aspect_impl(), 0.1f, 100.0f);

    vec3s eye = { { 0.0f, 0.0f, -3.0f } };
    vec3s target = { { 0.0f, 0.0f, 0.0f } };
    // WARNING: Y axis is inverted to compensate for the coordinate system (X+ right, Y+ down, Z+ forward).
    // This is a left-handed coordinate system with the Y axis inverted.
    vec3s world_up = { { 0.0f, -1.0f, 0.0f } };
    mat4s view = glms_lookat(eye, target, world_up);

    mat4s model = glms_mat4_identity();
    model = glms_scale(model, scale);
    model = glms_rotate_z(model, rotation.z);
    model = glms_rotate_y(model, rotation.y);
    model = glms_rotate_x(model, rotation.x);
    model = glms_translate(model, translation);

    rotation.z += 0.002f;
    frame_impl(proj, view, model);
}

void event(const sapp_event* event) {
    (void)event;
}

void cleanup(void) {
    cleanup_impl();
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
