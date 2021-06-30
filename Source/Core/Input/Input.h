#pragma once

#include "State.h"

#include <GLFW/glfw3.h>

#include "Keys.h"
#include "Buttons.h"

namespace Input {

inline static GLFWwindow* glfwWindow { nullptr };
inline static std::map<int, State> mouseButtons;
inline static std::map<int, State> keys;
inline static MouseWheelState mouseWheelState;

enum class Key {
	W = KEY_W,
	A = KEY_A,
	S = KEY_S,
	D = KEY_D,

	Space = KEY_SPACE,
	Esc = KEY_ESCAPE,

	Num1 = KEY_1,
	Num2 = KEY_2,
	Num3 = KEY_3,
	Num4 = KEY_4,
};

enum class Button {
	Left = MOUSE_BUTTON_LEFT,
	Right = MOUSE_BUTTON_RIGHT
};

void MouseWheelCallback(GLFWwindow* glfwWindow, double xOffset, double yOffset);
void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
void KeyboardKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);

void Create(GLFWwindow* window);

bool MouseButtonPressed(int button);
bool MouseButtonReleased(int button);
bool MouseButtonDown(int button);

bool MouseButtonPressed(Button button);
bool MouseButtonReleased(Button button);
bool MouseButtonDown(Button button);

bool KeyPressed(int button);
bool KeyReleased(int button);
bool KeyDown(int button);

bool KeyPressed(Key button);
bool KeyReleased(Key button);
bool KeyDown(Key button);

void BeginFrame();
void EndFrame();

MouseWheelState GetMouseWheelState();



}