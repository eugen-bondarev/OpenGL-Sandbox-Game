#include "Window.h"

Window::Window(Size size, const std::string& title) {
    glfwInit();
    glfwDefaultWindowHints();

    glfwWindow = glfwCreateWindow(size.width, size.height, title.c_str(), nullptr, nullptr);
    glfwMaximizeWindow(glfwWindow);
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(glfwWindow);
}

void Window::PollEvents() const {
    glfwPollEvents();
}

void Window::SwapBuffers() const {
    glfwSwapBuffers(glfwWindow);
}