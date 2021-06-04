#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_ColorPass;

uniform int u_Highlight;

void main() {
  if (u_Highlight == 1) {
    vec4 col = texture(u_ColorPass, out_Uv);
    out_Color = vec4(col.rgb * 1.3, col.a);
  } else {
    out_Color = texture(u_ColorPass, out_Uv);
  }
}