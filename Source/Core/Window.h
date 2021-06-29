#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/Structures.h"
#include "Maths/Maths.h"

enum class WindowMode {
	Fullscreen,
	Borderless,
	Windowed
};

struct WindowSettings {
	Vec2 size 			{ 1920, 1080 };
	WindowMode mode	{ WindowMode::Fullscreen };
	bool maximize		{ true };
	bool vSync			{ true };
};

class Window {
public:
	static void Create(WindowSettings windowSettings = {}, bool resizable = false, const std::string& title = "Naturaforge");
	static void Destroy();
	static void Shutdown();

	static bool ShouldClose();
	static void Clear();
	static void PollEvents();
	static void SwapBuffers();
	
	static void Minimize();
	static void Close();

	static Vec2 GetPosition();
	static Vec2 GetSize();
	static Mat4 GetSpace();
	static Vec2 GetMousePosition();

	static void BeginFrame();
	static void EndFrame();

	inline static GLFWwindow *GetGlfwWindow() {
		return glfwWindow;
	}

	static void Recreate(WindowSettings windowSettings = {});

private:
	inline static Vec2 size;
	inline static Mat4 space;
	inline static void CalculateSpace();

	inline static GLFWwindow *glfwWindow;

	inline static bool recreate { false };
	inline static WindowSettings newWindowSettings;

	Window(const Window &) = delete;
	Window operator=(const Window &) = delete;
};