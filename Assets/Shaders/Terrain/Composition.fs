#version 440 core

in vec2 out_Uv;

out vec4 out_Color;

uniform sampler2D u_ColorPassResult;
uniform sampler2D u_LightPassResult;

void main() {
    vec4 color = texture(u_ColorPassResult, out_Uv);
    float light = texture(u_LightPassResult, out_Uv).r;

    out_Color = vec4(color.xyz * light, color.a);
}