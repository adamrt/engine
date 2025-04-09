#include "renderable.h"

#include "cglm/struct/affine.h"
#include "cglm/struct/mat4.h"

#include "vertex.h"

mat4s renderable_model_matrix(transform_t transform) {
    mat4s model = glms_mat4_identity();
    model = glms_scale(model, transform.scale);
    model = glms_rotate_x(model, transform.rotation.x);
    model = glms_rotate_y(model, transform.rotation.y);
    model = glms_rotate_z(model, transform.rotation.z);
    model = glms_translate(model, transform.translation);
    return model;
}

renderable_t renderable_cube(void) {
    renderable_t renderable = { 0 };
    renderable.type = RENDERABLE_TYPE_MESH;

    int num_vertices = 3981;
    for (int i = 0; i < num_vertices; i += 3) {
        vertex_t v0 = vertices[i];
        vertex_t v1 = vertices[i + 1];
        vertex_t v2 = vertices[i + 2];

        triangle_t triangle = {
            .a = v0,
            .b = v1,
            .c = v2,
        };
        renderable.mesh.triangles[renderable.mesh.num_triangles++] = triangle;
    }
    renderable.transform = (transform_t) {
        .translation = { { 0.0f, 0.0f, 0.0f } },
        .rotation = { { 0.0f, 0.0f, 0.0f } },
        .scale = { { 1.0f, 1.0f, 1.0f } },
    };

    return renderable;
}
