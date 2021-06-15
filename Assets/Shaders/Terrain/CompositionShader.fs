#version 440 core

in vec2 out_UV;

out vec4 out_Color;

uniform sampler2D u_ColorPassResult;
uniform sampler2D u_LightPassResult;

void main() {
  vec4 pureColor = texture(u_ColorPassResult, out_UV);
  float light = texture(u_LightPassResult, out_UV).r;

  out_Color = vec4(pureColor.xyz * light, pureColor.a);
}