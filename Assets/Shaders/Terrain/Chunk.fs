#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_ColorPass;
uniform sampler2D u_LightPass;

void main() {
  float factor = texture(u_LightPass, out_Uv).r;
  vec4 color = texture(u_ColorPass, out_Uv);
  out_Color = vec4(color.xyz * factor, color.a);
}