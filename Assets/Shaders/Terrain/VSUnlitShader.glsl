#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec2 in_BlockData;

uniform mat4 u_ProjectionView;

void main() {
  gl_Position = u_ProjectionView * vec4(in_Position + in_BlockData.xy, 0.0, 1.0);
}