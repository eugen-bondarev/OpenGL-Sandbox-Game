#version 440 core

in vec2 out_UVs;

out vec4 out_Color;

uniform sampler2D u_ColorPassResult;
uniform sampler2D u_LightPassResult;

void main() {
  vec4 pureColor = texture(u_ColorPassResult, out_UVs);
  float light = texture(u_LightPassResult, out_UVs).r;

  out_Color = vec4(pureColor.xyz * light, pureColor.a);
}