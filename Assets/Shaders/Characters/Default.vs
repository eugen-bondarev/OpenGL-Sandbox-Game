#version 440 core

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out vec2 out_Uv;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

uniform float u_Frame1;
uniform float u_Frames_Vert;
uniform float u_Direction;

uniform float u_Weapon;

void main() {
  // float correction = -96.0 + u_Direction * 8.0;
  float correction = 16.0;

  if (u_Weapon != 1.0) {
    gl_Position = u_Proj * u_View * (u_Model * vec4(position, 0.0, 1.0) + vec4(vec2(correction, (u_Frames_Vert - u_Frame1) * -32.0) + vec2(0, 32.0), 0, 0));
  } else {
    gl_Position = u_Proj * u_View * (u_Model * vec4(position, 0.0, 1.0) + vec4(vec2(correction, 0), 0, 0));
  }
  out_Uv = uv;
}