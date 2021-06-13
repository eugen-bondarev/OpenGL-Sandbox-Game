#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec4 blockData;

out vec2 out_UV;
out vec2 out_Tile;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main() {
  vec2 POSITION = blockData.xy;

  gl_Position = u_Proj * u_View * vec4(position + POSITION, 0.0, 1.0);

  out_UV = uv;
  out_Tile = blockData.zw;
}