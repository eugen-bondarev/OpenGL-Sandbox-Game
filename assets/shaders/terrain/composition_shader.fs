#version 440 core

in vec2 out_UV;

out vec4 out_Color;

uniform sampler2D u_ColorPassResult;
uniform sampler2D u_LightPassResult;

void main() {
  vec4 pureColor = texture(u_ColorPassResult, out_UV);
  // float light = texture(u_LightPassResult, out_UV * vec2(1920.0 / 1366.0, 1080.0 / 768.0)).r;
  // float light = texture(u_LightPassResult, out_UV * vec2(1366.0 / 1920.0, 768.0 / 1080.0)).r;
  float light = texture(u_LightPassResult, out_UV).r;

  out_Color = vec4(pureColor.xyz * light, pureColor.a);
}