#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec4 in_BlockData;

out vec2 out_UV;
out vec2 out_Tile;
out float out_ToDiscard;

uniform mat4 u_Proj;
uniform mat4 u_View;

void main() {
  vec2 position = in_BlockData.xy;
  out_Tile = in_BlockData.zw;

  if (position == vec2(0, 0)) {
    out_ToDiscard = 1;
    gl_Position = vec4(0, 0, 0, 0);
  } else {
    out_ToDiscard = 0;
    gl_Position = u_Proj * u_View * vec4(in_Position + position, 0.0, 1.0);
    out_UV = in_UV;
  }  
}