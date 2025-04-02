@ctype mat4 mat4s
@ctype mat3 mat3s
@ctype vec4 vec4s
@ctype vec3 vec3s
@ctype vec2 vec2s

@vs vs
// Uniforms
layout(binding=0) uniform vs_primary_params {
    mat4 u_proj;
    mat4 u_view;
    mat4 u_model;
};

// Attributes
layout(location = 0) in vec3 a_position;
layout(location = 1) in vec4 a_color;

out vec4 v_color;

void main() {
    gl_Position = u_proj * u_view * u_model * vec4(a_position, 1.0);
    v_color = a_color;
}
@end

@fs fs
in vec4 v_color;
out vec4 frag_color;

void main() {
    frag_color = v_color;
}
@end


/* vertex shader */
@vs quad_vs
in vec3 a_position;
in vec2 a_uv;

out vec2 v_uv;

void main() {
    gl_Position = vec4(a_position, 1.0);
    v_uv = a_uv;
}
@end

/* fragment shader */
@fs quad_fs
layout(binding=0) uniform texture2D u_tex;
layout(binding=0) uniform sampler u_smp;

in vec2 v_uv;

out vec4 frag_color;

void main() {

    frag_color = texture(sampler2D(u_tex, u_smp), v_uv);
}
@end

@program primary vs      fs
@program quad    quad_vs quad_fs
