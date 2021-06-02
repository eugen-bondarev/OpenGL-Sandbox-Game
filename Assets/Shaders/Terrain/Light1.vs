#version 440 core

in vec2 position;
in vec2 uv;

out vec2 out_Uv;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform vec2 u_Positions[256];

void main() {
  gl_Position = u_Proj * u_View * vec4(position + u_Positions[gl_InstanceID], 0.0, 1.0);
  out_Uv = uv;
}