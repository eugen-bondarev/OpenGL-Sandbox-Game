#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UVs;
layout (location = 2) in vec2 in_InstancePosition;

out vec2 out_UVs;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main() {
  gl_Position = u_Proj * u_View * vec4(in_Position + in_InstancePosition, 0.0, 1.0);

  out_UVs = in_UVs;
}