#pragma once

#include <GLFW/glfw3.h>

namespace mw {
namespace Time {

extern float delta;
extern float currentTime;
extern float lastTime;
extern float fpsTimer;
extern float fps;

void BeginFrame();
void EndFrame();

float GetDelta();
int GetFps();
float GetTime();

}
}