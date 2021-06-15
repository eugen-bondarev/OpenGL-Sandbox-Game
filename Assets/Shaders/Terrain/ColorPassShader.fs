#version 440 core

in vec2 out_UV;
in vec2 out_Tile;
in float out_ToDiscard;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main() {
  if (out_ToDiscard == 1) {
    discard;
  }

  out_Color = texture(u_Texture, (out_UV + out_Tile) / vec2(6, 6));
}