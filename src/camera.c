#include "camera.h"

#include "cglm/struct/cam.h"

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

// Linked in the cpu/gpu_impl.c files.
float gfx_aspect(void);

void camera_init(void) {
    camera.position = (vec3s) { { 0.0f, 0.0f, -400.0f } };
    camera.target = (vec3s) { { 0.0f, 0.0f, 0.0f } };
    camera.world_up = (vec3s) { { 0.0f, 1.0f, 0.0f } };

    camera.view = glms_lookat(camera.position, camera.target, camera.world_up);
    camera.persp_proj = glms_perspective(45.0f, gfx_aspect(), 0.1f, 100.0f);
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
