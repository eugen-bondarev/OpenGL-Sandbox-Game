#version 440 core

in vec2 out_UV;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main() {
  out_Color = texture(u_Texture, out_UV);
}