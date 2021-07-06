#include "window/window.h"

#include "graphics_context.h"

#include "window/gui.h"
#include "window/input.h"

namespace mw {
namespace Window {

std::vector<std::function<void()>> callbacks = {};

void Create(Settings windowSettings, bool resizable, const std::string &title)
{
	size = windowSettings.size;

	glfwInit();

	glfwDefaultWindowHints();
	// glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	const GLFWvidmode *videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (windowSettings.mode == Mode::Borderless)
	{
		glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

		glfwWindow = glfwCreateWindow(videoMode->width, videoMode->height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}

	if (windowSettings.mode == Mode::Fullscreen)
	{
		glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}

	if (windowSettings.mode == Mode::Windowed)
	{
		glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
	}

	if (windowSettings.maximize)
	{
		glfwMaximizeWindow(glfwWindow);
	}

	glfwMakeContextCurrent(glfwWindow);

	GLint glewInitResult = glewInit();
	if (GLEW_OK != glewInitResult)
	{
		printf("ERROR: %s", glewGetErrorString(glewInitResult));
		exit(EXIT_FAILURE);
	}

	GraphicsContext::EnableTransparency();

	glEnable(GL_MULTISAMPLE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height)
	{
		if (width == 0 || height == 0)
		{
			return;
		}

		size.x = width;
		size.y = height;
		CalculateSpace();

		for (int i = 0; i < callbacks.size(); i++)
		{
			callbacks[i]();
		}
	});

	glfwSwapInterval(windowSettings.vSync);

	int currentWidth, currentHeight;
	glfwGetWindowSize(glfwWindow, &currentWidth, &currentHeight);
	size = {currentWidth, currentHeight};

	CalculateSpace();
}

void Destroy()
{
	glfwDestroyWindow(glfwWindow);
}

void Shutdown()
{
	glfwTerminate();
}

Vec2 GetPosition()
{
	Vec2i pos;
	glfwGetWindowPos(glfwWindow, &pos.x, &pos.y);

	return pos;
}

Vec2 GetSize()
{
	return size;
}

Mat4 GetSpace()
{
	return space;
}

Vec2 GetMousePosition()
{
	double x, y;
	glfwGetCursorPos(GetGlfwWindow(), &x, &y);
	return {x, y};
}

void CalculateSpace()
{
	space = Math::Ortho(-size.x / 2.0f, size.x / 2.0f, -size.y / 2.0f, size.y / 2.0f);
}

bool ShouldClose()
{
	return glfwWindowShouldClose(glfwWindow);
}

void Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void PollEvents()
{
	glfwPollEvents();
}

void SwapBuffers()
{
	glfwSwapBuffers(glfwWindow);
}

void BeginFrame()
{
	PollEvents();

	if (recreate)
	{
		Gui::Destroy();
		Destroy();

		Create(newWindowSettings);
		Gui::Create();
		Input::Create(GetGlfwWindow());

		recreate = false;
	}
}

void EndFrame()
{
	SwapBuffers();
}

void Minimize()
{
	glfwIconifyWindow(glfwWindow);
}

void Close()
{
	glfwSetWindowShouldClose(glfwWindow, true);
}

void Recreate(Settings windowSettings)
{
	recreate = true;
	newWindowSettings = windowSettings;
}

GLFWwindow *GetGlfwWindow()
{
	return glfwWindow;
}

}
}