#pragma once

#include "State.h"

#include <GLFW/glfw3.h>

#include "Keys.h"
#include "Buttons.h"

class Input {
public:
  inline static void Create(GLFWwindow* glfwWindow) {
    Input::glfwWindow = glfwWindow;

		glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
		glfwSetKeyCallback(glfwWindow, KeyboardKeyCallback);
  }

	static void MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods);
	static bool MouseButtonPressed(int button);
	static bool MouseButtonReleased(int button);
	static bool MouseButtonDown(int button);

	static void KeyboardKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods);
	static bool KeyPressed(int button);
	static bool KeyReleased(int button);
	static bool KeyDown(int button);

	static void BeginFrame();
	static void EndFrame();

private:
	inline static GLFWwindow* glfwWindow;

	inline static std::map<int, State> mouseButtons;
	inline static std::map<int, State> keys;

  Input();
};