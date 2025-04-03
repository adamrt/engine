#ifndef CAMERA_H_
#define CAMERA_H_

#include "cglm/types-struct.h"

void camera_init(void);
void camera_toggle(void);
mat4s camera_proj(void);
mat4s camera_view(void);

#endif // CAMERA_H_
