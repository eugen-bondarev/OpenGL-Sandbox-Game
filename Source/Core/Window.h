#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/Structures.h"
#include "Math/Math.h"

class Window {
public:
    Window(Size size = { 1920, 1080 }, const std::string& title = "Forgio");
    ~Window();

    bool ShouldClose() const;
    void PollEvents() const;
    void SwapBuffers() const;

    Size GetSize() const;
    Mat4 GetSpace() const;

private:
    Size size;
    Mat4 space;
    void CalculateSpace();

    GLFWwindow* glfwWindow;

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;
};