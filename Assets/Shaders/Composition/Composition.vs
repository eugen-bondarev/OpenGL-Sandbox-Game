#version 440 core

in vec2 position;
in vec2 uv;

out vec2 out_Uv;

void main() {
  gl_Position = vec4(position, 0.0, 1.0);
  out_Uv = uv;
}