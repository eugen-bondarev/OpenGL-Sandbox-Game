#version 440 core

in vec2 out_UV;
in float out_ToDiscard;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main() {
  if (out_ToDiscard == 1) {
    discard;
  }

  out_Color = texture(u_Texture, out_UV);
}