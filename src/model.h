#ifndef MODEL_H_
#define MODEL_H_

#include "cglm/types-struct.h"

typedef struct {
    vec3s translation;
    vec3s rotation;
    vec3s scale;
} transform_t;

mat4s model_matrix(transform_t transform);

#endif // MODEL_H_
