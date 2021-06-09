#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out vec2 out_Uv;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

void main() {
  gl_Position = u_Proj * u_View * u_Model * vec4(position, 0.0, 1.0);
  out_Uv = uv;
}