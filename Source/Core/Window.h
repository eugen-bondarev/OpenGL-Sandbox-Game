#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Util/Structures.h"
#include "Maths/Maths.h"

namespace Window {

enum class Mode {
	Fullscreen,
	Borderless,
	Windowed
};

struct Settings {
	Vec2 size 															{ 1920, 1080 };
	Mode mode																{ Mode::Fullscreen };
	bool maximize														{ true };
	bool vSync															{ true };
};

inline static GLFWwindow* glfwWindow 			{ nullptr };
inline static Vec2 size 									{ 0 };
inline static Mat4 space 									{ 0 };
inline static bool recreate 							{ false };
inline static Settings newWindowSettings 	{};

void Create(Settings windowSettings = {}, bool resizable = false, const std::string& title = "Naturaforge");
void Recreate(Settings windowSettings = {});

void Destroy();
void Shutdown();

bool ShouldClose();
void Clear();
void PollEvents();
void SwapBuffers();

void BeginFrame();
void EndFrame();

void Minimize();
void Close();

void CalculateSpace();

Vec2 GetSize();
Vec2 GetPosition();
Vec2 GetMousePosition();
Mat4 GetSpace();
GLFWwindow *GetGlfwWindow();

}