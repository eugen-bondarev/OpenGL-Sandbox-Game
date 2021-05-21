#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window {
public:
    Window(Size size = { 1920, 1080 }, const std::string& title = "Forgio");
    ~Window();

    bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

private:
    GLFWwindow* glfwWindow;

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;
};