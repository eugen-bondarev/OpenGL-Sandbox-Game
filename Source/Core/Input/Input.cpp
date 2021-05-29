#include "Input.h"

void Input::MouseButtonCallback(GLFWwindow* glfwWindow, int button, int action, int mods)
{
	if (action == GLFW_RELEASE) mouseButtons[button] = State::Released;
	if (action == GLFW_PRESS)   mouseButtons[button] = State::Pressed;
}

void Input::KeyboardKeyCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE) keys[key] = State::Released;
	if (action == GLFW_PRESS)   keys[key] = State::Pressed;
}

void Input::BeginFrame()
{
}

void Input::EndFrame()
{
	mouseButtons.clear();
	keys.clear();
}

bool Input::MouseButtonPressed(int button)
{
	if (mouseButtons.find(button) != mouseButtons.end())
	{
		return mouseButtons.at(button) == State::Pressed;
	}

	return false;
}

bool Input::MouseButtonReleased(int button)
{
	if (mouseButtons.find(button) != mouseButtons.end())
	{
		return mouseButtons.at(button) == State::Released;
	}

	return false;
}

bool Input::MouseButtonDown(int button)
{
	return glfwGetMouseButton(glfwWindow, button);
}

bool Input::KeyPressed(int key)
{
	if (keys.find(key) != keys.end())
	{
		return keys.at(key) == State::Pressed;
	}

	return false;
}

bool Input::KeyReleased(int key)
{
	if (keys.find(key) != keys.end())
	{
		return keys.at(key) == State::Released;
	}

	return false;
}

bool Input::KeyDown(int key)
{
	return glfwGetKey(glfwWindow, key);
}