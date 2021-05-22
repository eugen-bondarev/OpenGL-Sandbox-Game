#include "Window.h"

Window::Window(Size size, const std::string& title) {
    glfwInit();
    glfwDefaultWindowHints();

    glfwWindow = glfwCreateWindow(size.width, size.height, title.c_str(), nullptr, nullptr);
    glfwMaximizeWindow(glfwWindow);
    glfwMakeContextCurrent(glfwWindow);

    GLint GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult) 
    {
        printf("ERROR: %s",glewGetErrorString(GlewInitResult));
        exit(EXIT_FAILURE);
    }
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