#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main() {
    const vec4 color = texture(u_Texture, out_Uv);
    const float light = 1.0;

    out_Color = vec4(color.xyz * light, color.a);
}