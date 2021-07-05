#version 440 core

in vec2 out_UV;
in vec2 out_Tile;

out vec4 out_Color;

uniform sampler2D u_Texture;

#define AMOUNT_OF_TILES_X 18
#define AMOUNT_OF_TILES_Y 12

void main() {
  out_Color = texture(u_Texture, (out_UV + out_Tile) / vec2(AMOUNT_OF_TILES_X, AMOUNT_OF_TILES_Y));
}