#version 330 core

in vec2 coords;
out vec4 frag_color;

uniform sampler2D text;
uniform vec3 color;

void main() {
    vec4 sampled = vec4(1.f, 1.f, 1.f, texture(text, coords).r);
    frag_color = vec4(color, 1.f) * sampled;
}
