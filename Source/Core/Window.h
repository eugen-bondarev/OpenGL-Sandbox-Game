#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/Structures.h"
#include "Math/Math.h"

class Window {
public:
	enum class Mode {
		Fullscreen,
		Borderless,
		Windowed
	};

	static void Create(Size size = { 800, 600 }, Mode mode = Mode::Borderless, bool maximize = true, const std::string& title = "Forgio");
	static void Destroy();

	static bool ShouldClose();
	static void Clear();
	static void PollEvents();
	static void SwapBuffers();

	static Size GetSize();
	static Mat4 GetSpace();
	static Vec2 GetMousePosition();

	static void BeginFrame();
	static void EndFrame();

	inline static GLFWwindow *GetGlfwWindow() {
		return glfwWindow;
	}

private:
	inline static Size size;
	inline static Mat4 space;
	inline static void CalculateSpace();

	inline static GLFWwindow *glfwWindow;

	Window(const Window &) = delete;
	Window operator=(const Window &) = delete;
};