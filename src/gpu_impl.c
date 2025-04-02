#ifndef GPU_IMPL_H_
#define GPU_IMPL_H_

#include "cglm/types-struct.h"

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "sokol_log.h"

#include "shader.glsl.h"

#include "common.h"
#include "vertices.h"

static struct {
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

float aspect_impl(void) {
    return sapp_width() / (float)sapp_height();
}

void init_impl(void) {
    sg_desc desc = { 0 };
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    sg_buffer_desc vbuf_desc = { 0 };
    vbuf_desc.data = SG_RANGE(vertices);
    vbuf_desc.label = "triangle-vertices";
    state.bind.vertex_buffers[0] = sg_make_buffer(&vbuf_desc);

    sg_pipeline_desc pip_desc = { 0 };
    pip_desc.shader = sg_make_shader(primary_shader_desc(sg_query_backend()));
    pip_desc.layout.buffers[0].stride = sizeof(vertex_t);
    pip_desc.layout.attrs[ATTR_primary_a_position].format = SG_VERTEXFORMAT_FLOAT3;
    pip_desc.layout.attrs[ATTR_primary_a_position].offset = 0;
    pip_desc.layout.attrs[ATTR_primary_a_color].format = SG_VERTEXFORMAT_FLOAT4;
    pip_desc.layout.attrs[ATTR_primary_a_color].offset = offsetof(vertex_t, color);
    pip_desc.face_winding = SG_FACEWINDING_CW; // Left-handed
    pip_desc.cull_mode = SG_CULLMODE_BACK;
    pip_desc.label = "triangle-pipeline";
    state.pip = sg_make_pipeline(&pip_desc);

    sg_pass_action action = { 0 };
    action.colors[0].load_action = SG_LOADACTION_CLEAR;
    action.colors[0].clear_value = (sg_color) { 0.0f, 0.0f, 0.0f, 1.0f };
    state.pass_action = action;
}

void frame_impl(mat4s proj, mat4s view, mat4s model) {
    sg_pass pass = { 0 };
    pass.action = state.pass_action;
    pass.swapchain = sglue_swapchain();

    vs_primary_params_t vs_params = {
        .u_model = model,
        .u_view = view,
        .u_proj = proj,
    };

    sg_begin_pass(&pass);
    {
        int num_vertices = 36;
        sg_apply_pipeline(state.pip);
        sg_apply_bindings(&state.bind);
        sg_apply_uniforms(0, &SG_RANGE(vs_params));
        sg_draw(0, num_vertices, 1);
    }
    sg_end_pass();

    sg_commit();
}

void cleanup_impl(void) {
    sg_shutdown();
}

#endif // GPU_IMPL_H_
