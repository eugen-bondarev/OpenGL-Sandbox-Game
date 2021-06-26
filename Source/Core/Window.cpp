#include "Window.h"

#include "Werwel/GraphicsContext.h"

#include "Input/Input.h"
#include "Gui.h"

void Window::Create(WindowSettings windowSettings, bool resizable, const std::string &title) {
	Window::size = windowSettings.size;

	glfwInit();

	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE);

	const GLFWvidmode *videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (windowSettings.mode == WindowMode::Borderless) {
		glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

		glfwWindow = glfwCreateWindow(videoMode->width, videoMode->height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}

	if (windowSettings.mode == WindowMode::Fullscreen) {
		glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}

	if (windowSettings.mode == WindowMode::Windowed) {
		glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
	}

	if (windowSettings.maximize) {
		glfwMaximizeWindow(glfwWindow);
	}

	glfwMakeContextCurrent(glfwWindow);

	GLint glewInitResult = glewInit();
	if (GLEW_OK != glewInitResult) {
		printf("ERROR: %s", glewGetErrorString(glewInitResult));
		exit(EXIT_FAILURE);
	}

	Werwel::GraphicsContext::EnableTransparency();

	glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height) {
		if (width == 0 || height == 0) return;

		Window::size.x = width;
		Window::size.y = height;		
		Window::CalculateSpace();
	});

	glfwSwapInterval(windowSettings.vSync);

	int currentWidth, currentHeight;
	glfwGetWindowSize(glfwWindow, &currentWidth, &currentHeight);
	Window::size = { currentWidth, currentHeight };

	CalculateSpace();
}

void Window::Destroy() {
	glfwDestroyWindow(glfwWindow);
}

void Window::Shutdown() {
	glfwTerminate();
}

Vec2 Window::GetPosition() {
	Vec2i pos;
	glfwGetWindowPos(glfwWindow, &pos.x, &pos.y);

	return pos;
}

Vec2 Window::GetSize() {
	return size;
}

Mat4 Window::GetSpace() {
	return space;
}

Vec2 Window::GetMousePosition() {
	double x, y;
	glfwGetCursorPos(Window::GetGlfwWindow(), &x, &y);
	return {x, y};
}

void Window::CalculateSpace() {
	space = Math::Ortho(-size.x / 2.0f, size.x / 2.0f, -size.y / 2.0f, size.y / 2.0f);
}

bool Window::ShouldClose() {
	return glfwWindowShouldClose(glfwWindow);
}

void Window::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
}

void Window::PollEvents() {
	glfwPollEvents();
}

void Window::SwapBuffers() {
	glfwSwapBuffers(glfwWindow);
}

void Window::BeginFrame() {
	PollEvents();

	if (recreate) {
		Gui::Destroy();
		Window::Destroy();

		Window::Create(newWindowSettings);		
		Gui::Create();		
		Input::Create(Window::GetGlfwWindow());
		
		recreate = false;
	}
}

void Window::EndFrame() {
	SwapBuffers();
}

void Window::Close() {
	glfwSetWindowShouldClose(glfwWindow, true);
}

void Window::Recreate(WindowSettings windowSettings) {
	recreate = true;
	Window::newWindowSettings = windowSettings;
}