#include "Window.h"

void Window::Create(Size size, const std::string &title) {
	Window::size = size;

	glfwInit();
	glfwDefaultWindowHints();

	const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	glfwWindow = glfwCreateWindow(mode->width, mode->height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);

	glfwMaximizeWindow(glfwWindow);
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

	glfwSwapInterval(0);

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
