#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include "cglm/types-struct.h"
#include <stdint.h>

#define RENDERABLE_MAX_MESHES (8)

#define RENDERABLE_MAX_TRIANGLES (1400)
#define RENDERABLE_MAX_QUADS     (1024)
#define RENDERABLE_MAX_LINES     (16)

typedef enum {
    RENDERABLE_TYPE_NONE,
    RENDERABLE_TYPE_MESH,
    RENDERABLE_TYPE_LINE,
    RENDERABLE_TYPE_SPRITE,
    RENDERABLE_TYPE_COUNT
} renderable_type_e;

typedef struct {
    vec3s start, end;
    uint32_t color;
} line_t;

typedef struct {
    vec3s position;
    vec4s color;
} vertex_t;

typedef struct {
    vertex_t a, b, c;
} triangle_t;

typedef struct {
    vertex_t a, b, c, d;
} quad_t;

typedef struct {
    triangle_t triangles[RENDERABLE_MAX_TRIANGLES];
    quad_t quads[RENDERABLE_MAX_QUADS];

    int num_triangles;
    int num_quads;
} mesh_t;

typedef struct {
    vec3s translation;
    vec3s rotation;
    vec3s scale;
} transform_t;

typedef struct {
    renderable_type_e type;
    mesh_t mesh;
    line_t lines[RENDERABLE_MAX_LINES];
    int num_lines;
    transform_t transform;
} renderable_t;

mat4s renderable_model_matrix(transform_t transform);
renderable_t renderable_cube(void);

#endif // RENDERABLE_H_
