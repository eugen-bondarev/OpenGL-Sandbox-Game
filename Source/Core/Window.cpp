#include "Window.h"

void Window::Create(Size size, Mode mode, bool maximize, const std::string &title) {
	Window::size = size;

	glfwInit();
	glfwDefaultWindowHints();

	const GLFWvidmode *videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	if (mode == Mode::Borderless) {
		glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

		glfwWindow = glfwCreateWindow(videoMode->width, videoMode->height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}

	if (mode == Mode::Fullscreen) {
		glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
	}

	if (mode == Mode::Windowed) {
		glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
	}

	if (maximize) {
		glfwMaximizeWindow(glfwWindow);
	}

	glfwMakeContextCurrent(glfwWindow);

	GLint glewInitResult = glewInit();
	if (GLEW_OK != glewInitResult)
	{
		printf("ERROR: %s", glewGetErrorString(glewInitResult));
		exit(EXIT_FAILURE);
	}

	// Todo: move it somewhere..
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow *window, int width, int height) {
		Window::size.x = width;
		Window::size.y = height;
		Window::CalculateSpace();
	});

	glfwSwapInterval(1);

	int w, h;
	glfwGetWindowSize(glfwWindow, &w, &h);
	Window::size = {w, h};

	CalculateSpace();
}

void Window::Destroy() {
	glfwDestroyWindow(glfwWindow);
	glfwTerminate();
}

Size Window::GetSize() {
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
}

void Window::EndFrame() {
	SwapBuffers();
}
