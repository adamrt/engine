@vs vs
in vec3 v_position;
in vec4 v_color;

out vec4 a_color;

void main() {
    gl_Position = vec4(v_position, 1.0);
    a_color = v_color;
}
@end

@fs fs
in vec4 a_color;
out vec4 frag_color;

void main() {
    frag_color = a_color;
}
@end

@program primary vs fs
