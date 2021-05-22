#version 440 core

in vec3 position;
in vec3 uv;
in vec3 normal;

void main() {
    gl_Position = vec4(position, 1.0);
}