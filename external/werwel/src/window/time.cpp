#include "time.h"

namespace ww {
namespace Time {

float delta = 0;
float currentTime = 0;
float lastTime = 0;
float fpsTimer = 0;
float fps = 0;

void BeginFrame()
{
	currentTime = static_cast<float>(glfwGetTime());
	delta = (currentTime - lastTime);
	lastTime = currentTime;
}

void EndFrame()
{
	fpsTimer += delta;
	if (fpsTimer >= 0.3f)
	{
		fps = 1.0f / delta;
		fpsTimer = 0;
	}
}

float GetDelta()
{
	return delta;
}

int GetFps()
{
	return static_cast<int>(fps);
}

float GetTime()
{
	return static_cast<float>(glfwGetTime());
}

}
}