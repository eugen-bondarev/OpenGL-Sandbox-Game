#version 440 core

in vec2 out_UV;
in float out_Height;

out vec4 out_Color;

uniform vec3 u_Color0;
uniform vec3 u_Color1;

void main() {
  out_Color = vec4(u_Color0 + (u_Color1 - u_Color0) * out_Height, 1.0);
}