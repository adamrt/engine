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

renderable_t renderable_mesh(void) {
    renderable_t renderable = { 0 };
    renderable.type = RENDERABLE_TYPE_MESH;

    int num_triangle_vertices = 51 * 3;
    for (int i = 0; i < num_triangle_vertices; i += 3) {
        vertex_t v0 = tri_verts[i];
        vertex_t v1 = tri_verts[i + 1];
        vertex_t v2 = tri_verts[i + 2];
        renderable.mesh.triangles[renderable.mesh.num_triangles++] = (triangle_t) { v0, v1, v2 };
    }

    int num_quad_vertices = 363 * 4;
    for (int i = 0; i < num_quad_vertices; i += 4) {
        vertex_t v0 = quad_verts[i];
        vertex_t v1 = quad_verts[i + 1];
        vertex_t v2 = quad_verts[i + 2];
        vertex_t v3 = quad_verts[i + 3];
        renderable.mesh.quads[renderable.mesh.num_quads++] = (quad_t) { v0, v1, v2, v3 };
    }

    renderable.transform = (transform_t) {
        .translation = { { 0.0f, 0.0f, 0.0f } },
        .rotation = { { 0.0f, 0.0f, 0.0f } },
        .scale = { { 1.0f, 1.0f, 1.0f } },
    };

    return renderable;
}
