#ifndef CPU_IMPL_H_
#define CPU_IMPL_H_

#include "cglm/struct/mat4.h"
#include "cglm/types-struct.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "shader.glsl.h"

#include "common.h"
#include "vertices.h"

uint32_t color_black;
uint32_t color_white;
uint32_t color_red;
uint32_t color_green;
uint32_t color_blue;
uint32_t color_transparent;

#define swap(type, a, b) \
    do {                 \
        type _tmp = (a); \
        (a) = (b);       \
        (b) = _tmp;      \
    } while (0)

static void fb_clear(void);
static void fb_draw_pixel(int32_t x, int32_t y, uint32_t color);
static void fb_draw_line(int32_t x0, int32_t y0, int32_t x1, int32_t y1, uint32_t color);
static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a);

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
    sg_image fb_image;
    sg_sampler fb_sampler;

    uint32_t framebuffer[FB_SIZE];
    vec4s verts_to_render[256];
    int num_verts_to_render;
} state;

void _init_texture(void) {
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
}

void _init_fullscreen_quad(void) {
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
}

void _init_pipeline(void) {
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
}

float aspect_impl(void) {
    return FB_WIDTH / (float)FB_HEIGHT;
}

void init_impl(void) {
    sg_desc desc = { 0 };
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    state.num_verts_to_render = 0;

    _init_texture();
    _init_fullscreen_quad();
    _init_pipeline();

    color_black = rgb(0, 0, 0, 1);
    color_white = rgb(255, 255, 255, 1);
    color_red = rgb(255, 0, 0, 1);
    color_green = rgb(0, 255, 0, 1);
    color_blue = rgb(0, 0, 255, 1);
    color_transparent = rgb(0, 0, 0, 0);

    sg_pass_action action = { 0 };
    action.colors[0].load_action = SG_LOADACTION_CLEAR;
    action.colors[0].clear_value = (sg_color) { 0.0f, 0.0f, 0.0f, 1.0f };
    state.pass_action = action;
}

void frame_impl(mat4s proj, mat4s view, mat4s model) {
    fb_clear();

    mat4s mvp = glms_mat4_mul(proj, glms_mat4_mul(view, model));

    int num_vertices = 36;

    for (int i = 0; i < num_vertices; i++) {
        vertex_t vert = vertices[i];
        vec4s v = (vec4s) { { vert.position.x, vert.position.y, vert.position.z, 1.0f } };

        v = glms_mat4_mulv(mvp, v);
        if (v.w != 0.0f) {
            v.x /= v.w;
            v.y /= v.w;
            v.z /= v.w;
        }

        v.x = (v.x + 1.0f) * 0.5f * FB_WIDTH;
        v.y = (1.0f - v.y) * 0.5f * FB_HEIGHT;

        state.verts_to_render[state.num_verts_to_render++] = v;
    }

    fb_draw_line(255, 0, 0, 240, color_green);
    fb_draw_line(0, 0, 255, 240, color_red);

    for (int i = 0; i < state.num_verts_to_render; i += 3) {
        vec4s a = state.verts_to_render[i];
        vec4s b = state.verts_to_render[i + 1];
        vec4s c = state.verts_to_render[i + 2];

        fb_draw_line(a.x, a.y, b.x, b.y, color_white);
        fb_draw_line(b.x, b.y, c.x, c.y, color_white);
        fb_draw_line(c.x, c.y, a.x, a.y, color_white);
    }

    state.num_verts_to_render = 0;

    for (int y = 20; y < 50; y++) {
        for (int x = 20; x < 80; x++) {
            fb_draw_pixel(x, y, color_blue);
        }
    }

    // Upload CPU buffer -> GPU texture
    {
        sg_image_data img_data = { 0 };
        img_data.subimage[0][0] = SG_RANGE(state.framebuffer);
        sg_update_image(state.fb_image, &img_data);
    }

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

void cleanup_impl(void) {
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
    state.framebuffer[y * FB_WIDTH + x] = color;
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

static uint32_t rgb(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return ((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)g << 8) | ((uint32_t)r);
}

#endif // CPU_IMPL_H_
