#pragma once

#include "state.h"

#include <GLFW/glfw3.h>

#include "keys.h"
#include "buttons.h"

// #define NF_KEY_PRESSED(key) ::ww::Input::KeyPressed(key)
// #define NF_KEY_RELEASED(key) ::ww::Input::KeyReleased(key)
// #define NF_KEY_DOWN(key) ::ww::Input::KeyDown(key)

namespace ww {
namespace Input {

inline static GLFWwindow *glfwWindow{nullptr};
inline static std::map<int, State> mouseButtons;
inline static std::map<int, State> keys;
inline static MouseWheelState mouseWheelState;

enum class Key
{
	W = NF_KEY_W,
	A = NF_KEY_A,
	S = NF_KEY_S,
	D = NF_KEY_D,

	Space = NF_KEY_SPACE,
	Esc = NF_KEY_ESCAPE,

	Num1 = NF_KEY_1,
	Num2 = NF_KEY_2,
	Num3 = NF_KEY_3,
	Num4 = NF_KEY_4,
};

enum class Button
{
	Left = NF_MOUSE_BUTTON_LEFT,
	Right = NF_MOUSE_BUTTON_RIGHT
};

void MouseWheelCallback(GLFWwindow *glfwWindow, double xOffset, double yOffset);
void MouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods);
void KeyboardKeyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods);

void Create(GLFWwindow *window);

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
}