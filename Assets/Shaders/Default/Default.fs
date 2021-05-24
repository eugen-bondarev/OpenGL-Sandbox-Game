#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;
uniform vec2 u_Offset;
uniform vec2 u_AmountOfTiles;

void main() {
    vec2 coord = out_Uv / u_AmountOfTiles;

    const vec4 color = texture(u_Texture, coord + u_Offset);
    const float light = 1.0;

    out_Color = vec4(color.xyz * light, color.a);
}