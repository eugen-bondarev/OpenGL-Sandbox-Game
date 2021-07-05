#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_Texture;

uniform vec2 u_Frame;
uniform vec2 u_AmountOfFrames;
uniform float u_Direction;

void main() {
  out_Color = texture(u_Texture, (out_Uv + u_Frame) / u_AmountOfFrames);
}