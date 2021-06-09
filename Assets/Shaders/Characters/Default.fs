#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;

void main() {
  out_Color = texture(u_Texture, out_Uv);
}