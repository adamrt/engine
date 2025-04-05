#ifndef VERTICES_H_
#define VERTICES_H_

#include "cglm/types-struct.h"

typedef struct {
    vec3s position;
    vec4s color;
} vertex_t;

extern vertex_t vertices[];

#endif // VERTICES_H_
