#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;
uniform vec2 u_Offset;

void main() {
    vec2 coord = out_Uv;
    coord.x /= 9.0;
    coord.y /= 3.0;

    out_Color = texture(u_Texture, coord + u_Offset);
}