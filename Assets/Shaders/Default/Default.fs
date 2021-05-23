#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;
uniform vec2 u_Offset;
uniform vec2 u_AmountOfTiles;

void main() {
    vec2 coord = out_Uv;

    coord /= u_AmountOfTiles;

    out_Color = texture(u_Texture, coord + u_Offset);
}