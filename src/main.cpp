#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "shader.glsl.h"

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

static void init(void) {
    sg_desc desc = {};
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    float vertices[] = {
        0.0f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f
    };

    sg_buffer_desc vbuf_desc = {};
    vbuf_desc.data = SG_RANGE(vertices);
    vbuf_desc.label = "triangle-vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(&vbuf_desc);

    sg_pipeline_desc pip_desc = {};
    pip_desc.shader = sg_make_shader(primary_shader_desc(sg_query_backend()));
    pip_desc.layout.attrs[ATTR_primary_position].format = SG_VERTEXFORMAT_FLOAT3;
    pip_desc.layout.attrs[ATTR_primary_color0].format = SG_VERTEXFORMAT_FLOAT4;
    pip_desc.label = "triangle-pipeline";
    state.pip = sg_make_pipeline(&pip_desc);

    sg_pass_action action = {};
    action.colors[0].load_action = SG_LOADACTION_CLEAR;
    action.colors[0].clear_value = { 0.0f, 0.0f, 0.0f, 1.0f };
    state.pass_action = action;
}

void frame(void) {
    sg_pass pass = {};
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

    sapp_desc desc = {};

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
