#version 440 core

layout (location = 0) in mat2x4 in_Line;

uniform mat4 u_Proj;
uniform mat4 u_View;

out vec4 color;

void main() {
  vec4 line = in_Line[0];
  color = in_Line[1];

  gl_Position = u_Proj * u_View * vec4(gl_VertexID == 0 ? line.xy : line.zw, 0.0, 1.0);
}