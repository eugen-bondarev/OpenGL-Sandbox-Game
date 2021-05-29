#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_ColorPassResult;

void main() {
  out_Color = texture(u_ColorPassResult, out_Uv);
}