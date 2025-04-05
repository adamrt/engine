#ifndef SCENE_H_
#define SCENE_H_

#define SCENE_MAX_RENDERABLES (8)

#include "renderable.h"

typedef struct {
    renderable_t renderables[SCENE_MAX_RENDERABLES];
    int num_renderables;
} scene_t;

void scene_add_renderable(renderable_t renderable);
void scene_add_axis(void);
scene_t* scene_get(void);

#endif // SCENE_H_
