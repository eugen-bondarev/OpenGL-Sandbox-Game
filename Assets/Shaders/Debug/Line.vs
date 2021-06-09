#version 440 core

uniform mat4 u_Proj;
uniform mat4 u_View;

uniform vec4 u_Line;
uniform mat2x4 u_LineData;

out vec4 color;

void main() {
  vec4 line = vec4(u_LineData[0][0], u_LineData[0][1], u_LineData[0][2], u_LineData[0][3]);
  color = vec4(u_LineData[1][0], u_LineData[1][1], u_LineData[1][2], u_LineData[1][3]);

  if (gl_VertexID == 0) {
    gl_Position = u_Proj * u_View * vec4(line.xy, 0.0, 1.0);
  } else {
    gl_Position = u_Proj * u_View * vec4(line.zw, 0.0, 1.0);
  }
}