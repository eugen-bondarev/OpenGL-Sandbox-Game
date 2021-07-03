#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;

uniform float u_Frame;
uniform float u_Frame1;
uniform float u_Frames_Vert;
uniform float u_Frames_Hor;
uniform float u_Direction;

uniform float u_Weapon;

void main() {
  if (u_Weapon != 1.0) {
    out_Color = texture(u_Texture, ((out_Uv + vec2(u_Frame, u_Frame1)) / vec2(u_Frames_Hor, u_Frames_Vert)));
  } else {
    out_Color = texture(u_Texture, (out_Uv + vec2(u_Frame, u_Frame1)) / vec2(u_Frames_Hor, u_Frames_Vert));
  }
}