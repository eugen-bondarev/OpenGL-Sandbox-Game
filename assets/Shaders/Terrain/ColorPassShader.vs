#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec4 in_BlockData;

out vec2 out_UV;
out vec2 out_Tile;

uniform mat4 u_ProjectionView;

void main() {
  vec2 position = in_BlockData.xy;
  out_Tile = in_BlockData.zw;

  gl_Position = u_ProjectionView * vec4(in_Position + position, 0.0, 1.0);
  out_UV = in_UV;
}