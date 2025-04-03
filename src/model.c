#include "model.h"
#include "cglm/struct/affine.h"

mat4s model_matrix(transform_t transform) {
    mat4s model = glms_mat4_identity();
    model = glms_scale(model, transform.scale);
    model = glms_rotate_z(model, transform.rotation.z);
    model = glms_rotate_y(model, transform.rotation.y);
    model = glms_rotate_x(model, transform.rotation.x);
    model = glms_translate(model, transform.translation);
    return model;
}
