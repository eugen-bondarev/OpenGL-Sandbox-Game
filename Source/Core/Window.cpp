#include "Window.h"

Window::Window(Size size, const std::string& title) {
    this->size = size;

    glfwInit();
    glfwDefaultWindowHints();

    glfwWindow = glfwCreateWindow(size.width, size.height, title.c_str(), nullptr, nullptr);
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

    glfwSetWindowUserPointer(glfwWindow, this);
    glfwSetWindowSizeCallback(glfwWindow, [](GLFWwindow* window, int width, int height) {
        Window* self = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
        self->size.width = width;
        self->size.height = height;
        self->CalculateSpace();
    });

    CalculateSpace();
}

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

Size Window::GetSize() const {
    return size;
}

Mat4 Window::GetSpace() const {
    return space;
}

void Window::CalculateSpace() {
    space = Math::Ortho(-size.width / 2.0f, size.width / 2.0f, -size.height / 2.0f, size.height / 2.0f);
}

bool Window::ShouldClose() const {
    return glfwWindowShouldClose(glfwWindow);
}

void Window::Clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::PollEvents() const {
    glfwPollEvents();
}

void Window::SwapBuffers() const {
    glfwSwapBuffers(glfwWindow);
}