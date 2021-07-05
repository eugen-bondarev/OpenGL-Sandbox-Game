#pragma once

#include <GLFW/glfw3.h>

class Time {
public:
  inline static void BeginFrame() {
    currentTime = static_cast<float>(glfwGetTime());
    delta = (currentTime - lastTime);
    lastTime = currentTime;
  }

  inline static void EndFrame() {
    fpsTimer += delta;
    if (fpsTimer >= 0.3f) {
      fps = 1.0f / delta;
      fpsTimer = 0;
    }
  }

  inline static float GetDelta() {
    return delta;
  }

  inline static int GetFps() {
    return static_cast<int>(fps);
  }

  inline static float GetTime() {
    return static_cast<float>(glfwGetTime());
  }

private:
  Time();
  
	inline static float delta{0};

	inline static float currentTime{0};
	inline static float lastTime{0};

	inline static float fpsTimer{0};
	inline static float fps{0};
};