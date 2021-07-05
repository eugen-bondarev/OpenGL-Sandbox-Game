#version 440 core

#define BOOLEAN     float
#define IS_TRUE(x)  (x == 1.0)

layout (location = 0) in vec2 position;
layout (location = 1) in vec2 uv;

out vec2 out_Uv;

uniform mat4    u_Proj;
uniform mat4    u_View;
uniform mat4    u_Model;

uniform vec2    u_Frame;
uniform vec2    u_AmountOfFrames;

uniform float   u_Direction;
uniform BOOLEAN u_Weapon;

void main() {
  vec2 correction = vec2(24, 0);

  if (!IS_TRUE(u_Weapon)) {
    correction.y = (u_AmountOfFrames.y - u_Frame.y - 1) * -32.0;
  }

  gl_Position = u_Proj * u_View * (u_Model * vec4(position, 0.0, 1.0) + vec4(correction, 0, 0));

  out_Uv = uv;
}