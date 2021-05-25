#version 440 core

in vec2 position;
in vec2 uv;

out vec2 out_Uv;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;
uniform vec2 u_Pos;

void main() {
    gl_Position = u_Proj * u_View * u_Model * vec4(position + u_Pos, 0.0, 1.0);

    out_Uv = uv;
}