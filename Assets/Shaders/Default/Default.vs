#version 440 core

in vec3 position;
in vec2 uv;
in vec3 normal;

out vec2 out_Uv;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec2 u_Pos;

void main() {
    gl_Position = u_Proj * u_View * u_Model * vec4(position, 1.0);

    out_Uv = uv;
}