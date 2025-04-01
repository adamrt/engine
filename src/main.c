#include "cglm/types-struct.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "shader.glsl.h"

typedef struct {
    vec3s position;
    vec4s color;
} vertex_t;

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

static void init(void) {
    sg_desc desc = { 0 };
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    // clang-format off
    // Right-handed triangle
    vertex_t vertices[] = {
        { { { 0.0f,  1.0f, 0.0f} }, {{ 1.0f, 0.0f, 0.0f, 1.0f }}}, // top - red
        { { { 1.0f, -1.0f, 0.0f} }, {{ 0.0f, 1.0f, 0.0f, 1.0f }}}, // bottom right - green
        { { {-1.0f, -1.0f, 0.0f} }, {{ 0.0f, 0.0f, 1.0f, 1.0f }}}, // bottom left - blue
    };
    // clang-format on

    sg_buffer_desc vbuf_desc = { 0 };
    vbuf_desc.data = SG_RANGE(vertices);
    vbuf_desc.label = "triangle-vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(&vbuf_desc);

    sg_pipeline_desc pip_desc = { 0 };
    pip_desc.shader = sg_make_shader(primary_shader_desc(sg_query_backend()));
    pip_desc.layout.buffers[0].stride = sizeof(vertex_t);
    pip_desc.layout.attrs[ATTR_primary_v_position].format = SG_VERTEXFORMAT_FLOAT3;
    pip_desc.layout.attrs[ATTR_primary_v_position].offset = 0;
    pip_desc.layout.attrs[ATTR_primary_v_color].format = SG_VERTEXFORMAT_FLOAT4;
    pip_desc.layout.attrs[ATTR_primary_v_color].offset = offsetof(vertex_t, color);
    pip_desc.face_winding = SG_FACEWINDING_CW; // Left-handed
    pip_desc.cull_mode = SG_CULLMODE_BACK;
    pip_desc.label = "triangle-pipeline";
    state.pip = sg_make_pipeline(&pip_desc);

    sg_pass_action action = { 0 };
    action.colors[0].load_action = SG_LOADACTION_CLEAR;
    action.colors[0].clear_value = (sg_color) { 0.0f, 0.0f, 0.0f, 1.0f };
    state.pass_action = action;
}

void frame(void) {
    sg_pass pass = { 0 };
    pass.action = state.pass_action;
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    {
        sg_apply_pipeline(state.pip);
        sg_apply_bindings(&state.bind);
        sg_draw(0, 3, 1);
    }
    sg_end_pass();

    sg_commit();
}

void event(const sapp_event* event) {
    (void)event;
}

void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    sapp_desc desc = { 0 };

    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = event;
    desc.width = 640;
    desc.height = 480;
    desc.window_title = "Engine";
    desc.icon.sokol_default = true;
    desc.logger.func = slog_func;
    return desc;
}
