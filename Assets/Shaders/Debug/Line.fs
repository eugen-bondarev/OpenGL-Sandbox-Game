#version 440 core

in vec4 color;

out vec4 out_Color;

uniform vec4 u_Color;

void main() {
  out_Color = color;
}