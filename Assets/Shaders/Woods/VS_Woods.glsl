#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec2 in_WorldPosition;

out vec2 out_UV;

uniform mat4 u_ProjectionView;

void main() {
  gl_Position = u_ProjectionView * vec4(in_Position + in_WorldPosition, 0.0, 1.0);

  out_UV = in_UV;
}