#version 440 core

in vec2 out_Uv;
in vec2 out_Tile;

out vec4 out_Color;

uniform sampler2D image;

void main() {
  out_Color = texture(image, (out_Uv + out_Tile) / vec2(6, 6));
}