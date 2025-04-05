#ifndef CPU_IMPL_H_
#define CPU_IMPL_H_

#include "cglm/struct/mat4.h"
#include "cglm/types-struct.h"

#include "renderable.h"
#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "shader.glsl.h"

#include "common.h"
#include "scene.h"
#include "vertex.h"

// Face represents a transformed triangle that can be ordered.
typedef struct {
    vec4s a, b, c;
    vec4s color_a, color_b, color_c;
    float avg_z;
} face_t;

#define swap(type, a, b) \
    do {                 \
        type _tmp = (a); \
        (a) = (b);       \
        (b) = _tmp;      \
    } while (0)

static void fb_clear(void);
static void fb_draw_pixel(int32_t x, int32_t y, uint32_t color);
static void fb_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
static void fb_draw_triangle_gouraud(vec4s v0, vec4s v1, vec4s v3, vec4s ca, vec4s cb, vec4s cc);
static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
static int sort_faces(const void* a, const void* b);

static struct {
    uint32_t framebuffer[FB_SIZE];

    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;

    // Fullscreen quad
    sg_image fb_image;
    sg_sampler fb_sampler;

    // Renderables
    face_t faces_to_render[256];
    int num_faces;
} state;

float gfx_aspect(void) {
    return FB_WIDTH / (float)FB_HEIGHT;
}

void gfx_init(void) {
    sg_desc desc = { 0 };
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    // Initialize framebuffer/texture
    sg_image_desc img_desc = { 0 };
    img_desc.width = FB_WIDTH;
    img_desc.height = FB_HEIGHT;
    img_desc.pixel_format = SG_PIXELFORMAT_RGBA8;
    img_desc.usage = SG_USAGE_STREAM; // we update it every frame
    img_desc.label = "cpu-framebuffer";
    state.fb_image = sg_make_image(&img_desc);

    sg_sampler_desc smp_desc = { 0 };
    smp_desc.min_filter = SG_FILTER_NEAREST;
    smp_desc.mag_filter = SG_FILTER_NEAREST;
    smp_desc.wrap_u = SG_WRAP_CLAMP_TO_EDGE;
    smp_desc.wrap_v = SG_WRAP_CLAMP_TO_EDGE;
    state.fb_sampler = sg_make_sampler(&smp_desc);

    // Initialize fullscreen quad
    // clang-format off
    float quad_vertices[] = {
        // position     uv
        -1.0f,  1.0f,   0.0f, 0.0f,
         1.0f,  1.0f,   1.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 1.0f,

        -1.0f,  1.0f,   0.0f, 0.0f,
         1.0f, -1.0f,   1.0f, 1.0f,
        -1.0f, -1.0f,   0.0f, 1.0f
    };
    // clang-format on

    sg_buffer_desc vbuf_desc = { 0 };
    vbuf_desc.data = SG_RANGE(quad_vertices);
    vbuf_desc.label = "quad-vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(&vbuf_desc);

    sg_shader shd = sg_make_shader(quad_shader_desc(sg_query_backend()));
    sg_pipeline_desc pip_desc = { 0 };
    pip_desc.shader = shd;
    pip_desc.layout.attrs[ATTR_quad_a_position].format = SG_VERTEXFORMAT_FLOAT2;
    pip_desc.layout.attrs[ATTR_quad_a_uv].format = SG_VERTEXFORMAT_FLOAT2;
    pip_desc.label = "quad-pipeline";
    state.pip = sg_make_pipeline(&pip_desc);

    // Assign the texture as a fragment shader image slot.
    state.bind.images[IMG_u_tex] = state.fb_image;
    state.bind.samplers[SMP_u_smp] = state.fb_sampler;

    // Clear to black
    state.pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    state.pass_action.colors[0].clear_value = (sg_color) { 0.0f, 0.0f, 0.0f, 1.0f };

    sg_pass_action action = { 0 };
    action.colors[0].load_action = SG_LOADACTION_CLEAR;
    action.colors[0].clear_value = (sg_color) { 0.0f, 0.0f, 0.0f, 1.0f };
    state.pass_action = action;
}

void gfx_frame(mat4s proj, mat4s view, mat4s model, scene_t* scene) {
    state.num_faces = 0;
    fb_clear();

    mat4s vertices_mvp = glms_mat4_mul(proj, glms_mat4_mul(view, model));

    int num_vertices = 36;

    for (int i = 0; i <= num_vertices - 3; i += 3) {
        vertex_t v0 = vertices[i];
        vertex_t v1 = vertices[i + 1];
        vertex_t v2 = vertices[i + 2];

        vec4s a = (vec4s) { { v0.position.x, v0.position.y, v0.position.z, 1.0f } };
        vec4s b = (vec4s) { { v1.position.x, v1.position.y, v1.position.z, 1.0f } };
        vec4s c = (vec4s) { { v2.position.x, v2.position.y, v2.position.z, 1.0f } };

        a = glms_mat4_mulv(vertices_mvp, a);
        b = glms_mat4_mulv(vertices_mvp, b);
        c = glms_mat4_mulv(vertices_mvp, c);

        // Perspective Divide. Works with ortho as well since W is always 1.0.
        // FIXME: Remove for ortho performance since its a essentially a no-op.
        if (a.w != 0.0f) {
            a.x /= a.w;
            a.y /= a.w;
            a.z /= a.w;
        }
        if (b.w != 0.0f) {
            b.x /= b.w;
            b.y /= b.w;
            b.z /= b.w;
        }
        if (c.w != 0.0f) {
            c.x /= c.w;
            c.y /= c.w;
            c.z /= c.w;
        }

        // Convert from NDC [-1, 1] to framebuffer coordinates [0, FB_WIDTH/HEIGHT]
        // +1 is for [-1, 1] to [0, 2] and *0.5f is for [0, 2] to [0, 1].
        a.x = (1.0f + a.x) * 0.5f * FB_WIDTH;
        a.y = (1.0f + a.y) * 0.5f * FB_HEIGHT;
        b.x = (1.0f + b.x) * 0.5f * FB_WIDTH;
        b.y = (1.0f + b.y) * 0.5f * FB_HEIGHT;
        c.x = (1.0f + c.x) * 0.5f * FB_WIDTH;
        c.y = (1.0f + c.y) * 0.5f * FB_HEIGHT;

        face_t face = {
            .a = a,
            .b = b,
            .c = c,
            .color_a = v0.color,
            .color_b = v1.color,
            .color_c = v2.color,
            .avg_z = (face.a.z + face.b.z + face.c.z) / 3.0f,
        };
        state.faces_to_render[state.num_faces++] = face;
    }

    // Sort back to front for painters algorithm.
    qsort(state.faces_to_render, state.num_faces, sizeof(face_t), sort_faces);

    // Draw triangles
    for (int i = 0; i < state.num_faces; i++) {
        fb_draw_triangle_gouraud(state.faces_to_render[i].a, state.faces_to_render[i].b, state.faces_to_render[i].c, state.faces_to_render[i].color_a, state.faces_to_render[i].color_b, state.faces_to_render[i].color_c);
    }

    for (int i = 0; i < scene->num_renderables; i++) {
        renderable_t* renderable = &scene->renderables[i];
        mat4s renderable_model = renderable_model_matrix(renderable->transform);
        mat4s renderable_mvp = glms_mat4_mul(proj, glms_mat4_mul(view, renderable_model));

        if (renderable->type == RENDERABLE_TYPE_LINE) {
            for (int j = 0; j < renderable->num_lines; j++) {
                line_t line = renderable->lines[j];
                vec4s a = (vec4s) { { line.start.x, line.start.y, line.start.z, 1.0f } };
                vec4s b = (vec4s) { { line.end.x, line.end.y, line.end.z, 1.0f } };

                a = glms_mat4_mulv(renderable_mvp, a);
                b = glms_mat4_mulv(renderable_mvp, b);

                if (a.w != 0.0f) {
                    a.x /= a.w;
                    a.y /= a.w;
                    a.z /= a.w;
                }
                if (b.w != 0.0f) {
                    b.x /= b.w;
                    b.y /= b.w;
                    b.z /= b.w;
                }

                a.x = (a.x + 1.0f) * 0.5f * FB_WIDTH;
                a.y = (1.0f - a.y) * 0.5f * FB_HEIGHT;
                b.x = (b.x + 1.0f) * 0.5f * FB_WIDTH;
                b.y = (1.0f - b.y) * 0.5f * FB_HEIGHT;

                fb_draw_line(a.x, a.y, b.x, b.y, line.color);
            }
        }
    }

    // Draw debug square for orientation. Remove later.
    for (int y = 20; y < 50; y++) {
        for (int x = 20; x < 80; x++) {
            fb_draw_pixel(x, y, color_blue);
        }
    }

    // Upload CPU buffer -> GPU texture
    sg_image_data img_data = { 0 };
    img_data.subimage[0][0] = SG_RANGE(state.framebuffer);
    sg_update_image(state.fb_image, &img_data);

    // Render
    sg_pass pass = { 0 };
    pass.action = state.pass_action;
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);
    {
        sg_apply_pipeline(state.pip);
        sg_apply_bindings(&state.bind);
        sg_draw(0, 6, 1);
    }
    sg_end_pass();
    sg_commit();
}

void gfx_cleanup(void) {
    sg_destroy_image(state.fb_image);
    sg_destroy_sampler(state.fb_sampler);
    sg_destroy_pipeline(state.pip);
    sg_destroy_buffer(state.bind.vertex_buffers[0]);
    sg_shutdown();
}

// using memset will use transparent black, not 0x8000.
static void fb_clear(void) {
    for (int i = 0; i < FB_SIZE; i++) {
        state.framebuffer[i] = color_black;
    }
}

static void fb_draw_pixel(int32_t x, int32_t y, uint32_t color) {
    if (x < 0 || x >= FB_WIDTH || y < 0 || y >= FB_HEIGHT) {
        return;
    }
    state.framebuffer[(FB_HEIGHT - y - 1) * FB_WIDTH + x] = color;
}

// draw_line uses the Bresenham's line algorithm
static void fb_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color) {
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        swap(int32_t, x0, y0);
        swap(int32_t, x1, y1);
    }

    if (x0 > x1) {
        swap(int32_t, x0, x1);
        swap(int32_t, y0, y1);
    }

    int32_t dx = x1 - x0;
    int32_t dy = abs(y1 - y0);
    int32_t error = dx / 2;
    int32_t ystep = (y0 < y1) ? 1 : -1;
    int32_t y = y0;

    for (int32_t x = x0; x <= x1; x++) {
        if (steep) {
            fb_draw_pixel(y, x, color);
        } else {
            fb_draw_pixel(x, y, color);
        }
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

static void fb_draw_triangle_gouraud(vec4s v0, vec4s v1, vec4s v3, vec4s ca, vec4s cb, vec4s cc) {
    // Bounding box
    int min_x = (int)fmaxf(0.0f, floorf(fminf(v0.x, fminf(v1.x, v3.x))));
    int max_x = (int)fminf(FB_WIDTH - 1, ceilf(fmaxf(v0.x, fmaxf(v1.x, v3.x))));
    int min_y = (int)fmaxf(0.0f, floorf(fminf(v0.y, fminf(v1.y, v3.y))));
    int max_y = (int)fminf(FB_HEIGHT - 1, ceilf(fmaxf(v0.y, fmaxf(v1.y, v3.y))));

    float dy13 = v1.y - v3.y;
    float dx03 = v0.x - v3.x;
    float dx31 = v3.x - v1.x;
    float dy03 = v0.y - v3.y;
    float dy30 = v3.y - v0.y;

    // Precompute edge functions
    float denom = (dy13) * (dx03) + (dx31) * (dy03);
    if (denom == 0.0f)
        return; // Degenerate

    for (int y = min_y; y <= max_y; y++) {
        for (int x = min_x; x <= max_x; x++) {
            float px = (float)x + 0.5f;
            float py = (float)y + 0.5f;

            // Barycentric coordinates
            float w1 = ((dy13) * (px - v3.x) + (dx31) * (py - v3.y)) / denom;
            float w2 = ((dy30) * (px - v3.x) + (dx03) * (py - v3.y)) / denom;
            float w3 = 1.0f - w1 - w2;

            // Inside triangle?
            if (w1 < 0 || w2 < 0 || w3 < 0)
                continue;

            // Interpolated color
            float r = ca.r * w1 + cb.r * w2 + cc.r * w3;
            float g = ca.g * w1 + cb.g * w2 + cc.g * w3;
            float b = ca.b * w1 + cb.b * w2 + cc.b * w3;

            fb_draw_pixel(x, y, rgb((uint8_t)(r * 255), (uint8_t)(g * 255), (uint8_t)(b * 255), 255));
        }
    }
}

static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | ((uint32_t)r);
}

// Painters algo
static int sort_faces(const void* a, const void* b) {
    const face_t* fa = (const face_t*)a;
    const face_t* fb = (const face_t*)b;
    if (fa->avg_z < fb->avg_z)
        return 1;
    if (fa->avg_z > fb->avg_z)
        return -1;
    return 0;
}

#endif // CPU_IMPL_H_
