#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;

uniform float u_Frame;
uniform float u_Direction;

void main() {
  out_Color = texture(u_Texture, ((out_Uv + vec2(u_Frame, 0)) / vec2(u_Direction * 14, 1)));
}