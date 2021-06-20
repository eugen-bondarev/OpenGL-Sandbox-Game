#pragma once

#include <GLFW/glfw3.h>

class Time {
public:
  inline static float delta { 0 };

  inline static void BeginFrame() {
    currentTime = static_cast<float>(glfwGetTime());
    delta = nextDelta;
    nextDelta = (currentTime - lastTime);
    lastTime = currentTime;
  }

  inline static void EndFrame() {
    fpsTimer += nextDelta;
    if (fpsTimer >= 1.0f) {
      fps = 1.0f / nextDelta;
      fpsTimer = 0;
    }
  }

  inline static float GetDelta() {
    return nextDelta;
  }

  inline static int GetFps() {
    return static_cast<int>(fps);
  }

  inline static float GetTime() {
    return static_cast<float>(glfwGetTime());
  }

	inline static float nextDelta{0};
private:
  Time();

	inline static float currentTime{0};
	inline static float lastTime{0};

	inline static float fpsTimer{0};
	inline static float fps{0};
};