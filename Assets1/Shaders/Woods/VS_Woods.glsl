#version 440 core

layout (location = 0) in vec2 in_Position;
layout (location = 1) in vec2 in_UV;
layout (location = 2) in vec2 in_WorldPosition;

out vec2 out_UV;

uniform mat4 u_ProjectionView;
uniform mat4 u_Model;

void main() {
  gl_Position = u_ProjectionView * ((u_Model * vec4(in_Position, 0.0, 1.0)) + vec4(in_WorldPosition, 0, 0));

  out_UV = in_UV;
}