#include "scene.h"
#include "common.h"
#include "renderable.h"

static scene_t scene = { 0 };

scene_t* scene_get(void) {
    return &scene;
}

void scene_add_renderable(renderable_t renderable) {
    if (scene.num_renderables < SCENE_MAX_RENDERABLES) {
        scene.renderables[scene.num_renderables++] = renderable;
    }
}

void scene_add_axis(void) {
    vec3s origin = { { 0.0f, 0.0f, 0.0f } };
    vec3s x_axis = { { 0.2f, 0.0f, 0.0f } };
    vec3s y_axis = { { 0.0f, -0.2f, 0.0f } };
    vec3s z_axis = { { 0.0f, 0.0f, 0.2f } };

    renderable_t renderable = { 0 };
    renderable.type = RENDERABLE_TYPE_LINE;
    renderable.transform.translation = (vec3s) { { 0, 0, 0 } };
    renderable.transform.rotation = (vec3s) { { 0, 0, 0 } };
    renderable.transform.scale = (vec3s) { { 1, 1, 1 } };

    renderable.lines[renderable.num_lines++] = (line_t) { origin, x_axis, color_red };
    renderable.lines[renderable.num_lines++] = (line_t) { origin, y_axis, color_green };
    renderable.lines[renderable.num_lines++] = (line_t) { origin, z_axis, color_blue };

    scene_add_renderable(renderable);
}
