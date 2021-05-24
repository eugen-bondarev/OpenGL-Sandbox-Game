#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/Structures.h"
#include "Math/Math.h"

class Window {
public:
    static void Create(Size size = { 1920, 1080 }, const std::string& title = "Forgio");
    static void Destroy();

    static bool ShouldClose();
    static void Clear();
    static void PollEvents();
    static void SwapBuffers();
    static bool KeyPressed(int key);
    static Size GetSize();
    static Mat4 GetSpace();

    inline static GLFWwindow* GetGlfwWindow() {
        return glfwWindow;
    }

private:
    inline static Size size;
    inline static Mat4 space;
    inline static void CalculateSpace();

    inline static GLFWwindow* glfwWindow;

    Window(const Window&) = delete;
    Window operator=(const Window&) = delete;
};