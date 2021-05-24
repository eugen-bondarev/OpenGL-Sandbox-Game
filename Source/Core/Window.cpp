#include "Window.h"

void Window::Create(Size size, const std::string& title) {
    Window::size = size;

    glfwInit();
    glfwDefaultWindowHints();

    glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    glfwMaximizeWindow(glfwWindow);
    glfwMakeContextCurrent(glfwWindow);

    GLint glewInitResult = glewInit();
    if (GLEW_OK != glewInitResult) {
        printf("ERROR: %s",glewGetErrorString(glewInitResult));
        exit(EXIT_FAILURE);
    }

    // Todo: move it somewhere..
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height) {
        Window::size.x = width;
        Window::size.y = height;
        Window::CalculateSpace();
    });

    CalculateSpace();
}

bool Window::KeyPressed(int key) {
    return glfwGetKey(glfwWindow, key);
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