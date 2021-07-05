#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;

out vec2 out_UV;

void main() {
  gl_Position = vec4(in_Position, 0.0, 1.0);
  out_UV = in_UV;
}