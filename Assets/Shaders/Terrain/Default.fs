#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D image;
uniform vec2 u_Tile;

void main() {
  out_Color = texture(image, (out_Uv + u_Tile) / vec2(9, 5));
}