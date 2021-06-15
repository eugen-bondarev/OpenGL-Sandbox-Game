#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec2 in_InstancePosition;

out vec2 out_UV;
out float out_ToDiscard;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main() {
  if (in_Position == vec2(0, 0)) {
    out_ToDiscard = 1;
    gl_Position = vec4(0, 0, 0, 0);
  } else {
    out_ToDiscard = 0;
    gl_Position = u_Proj * u_View * vec4(in_Position + in_InstancePosition, 0.0, 1.0);
    out_UV = in_UV;
  }
}