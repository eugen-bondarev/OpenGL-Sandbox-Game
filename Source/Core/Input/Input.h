#pragma once

#include "State.h"

#include <GLFW/glfw3.h>

#include "Keys.h"
#include "Buttons.h"

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

class Input {
public:
  inline static void Create(GLFWwindow* glfwWindow) {
    Input::glfwWindow = glfwWindow;

		mouseButtons.clear();
		keys.clear();

		glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
		glfwSetKeyCallback(glfwWindow, KeyboardKeyCallback);
  }

	static void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
	static bool MouseButtonPressed(int button);
	static bool MouseButtonReleased(int button);
	static bool MouseButtonDown(int button);

	static bool MouseButtonPressed(Button button);
	static bool MouseButtonReleased(Button button);
	static bool MouseButtonDown(Button button);

	static void KeyboardKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
	static bool KeyPressed(int button);
	static bool KeyReleased(int button);
	static bool KeyDown(int button);
	
	static bool KeyPressed(Key button);
	static bool KeyReleased(Key button);
	static bool KeyDown(Key button);

	static void BeginFrame();
	static void EndFrame();

private:
	inline static GLFWwindow* glfwWindow;

	inline static std::map<int, State> mouseButtons;
	inline static std::map<int, State> keys;

  Input();
};