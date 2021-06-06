#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec2 positions;

out vec2 out_Uv;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main() {
  gl_Position = u_Proj * u_View * vec4(position + positions, 0.0, 1.0);
  out_Uv = uv;
}