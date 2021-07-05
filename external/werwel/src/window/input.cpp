#include "window/input.h"

#include "imgui/imgui.h"

namespace ww {

void Input::Create(GLFWwindow *window)
{
	glfwWindow = window;

	mouseButtons.clear();
	keys.clear();

	glfwSetMouseButtonCallback(glfwWindow, MouseButtonCallback);
	glfwSetKeyCallback(glfwWindow, KeyboardKeyCallback);
	glfwSetScrollCallback(glfwWindow, MouseWheelCallback);
}

void Input::MouseButtonCallback(GLFWwindow *glfwWindow, int button, int action, int mods)
{
	if (action == GLFW_RELEASE)
		mouseButtons[button] = State::Released;
	if (action == GLFW_PRESS)
		mouseButtons[button] = State::Pressed;
}

void Input::KeyboardKeyCallback(GLFWwindow *glfwWindow, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		keys[key] = State::Released;
	if (action == GLFW_PRESS)
		keys[key] = State::Pressed;
}

void Input::MouseWheelCallback(GLFWwindow *glfwWindow, double xOffset, double yOffset)
{
	if (yOffset > 0)
	{
		mouseWheelState = MouseWheelState::Up;
	}
	else if (yOffset < 0)
	{
		mouseWheelState = MouseWheelState::Down;
	}
}

void Input::BeginFrame()
{
}

void Input::EndFrame()
{
	mouseButtons.clear();
	keys.clear();
	mouseWheelState = MouseWheelState::Idle;
}

MouseWheelState Input::GetMouseWheelState()
{
	return mouseWheelState;
}

bool Input::MouseButtonPressed(int button)
{
	if (mouseButtons.find(button) != mouseButtons.end())
	{
		return !ImGui::GetIO().WantCaptureMouse && mouseButtons.at(button) == State::Pressed;
	}

	return false;
}

bool Input::MouseButtonReleased(int button)
{
	if (mouseButtons.find(button) != mouseButtons.end())
	{
		return !ImGui::GetIO().WantCaptureMouse && mouseButtons.at(button) == State::Released;
	}

	return false;
}

bool Input::MouseButtonDown(int button)
{
	return !ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(glfwWindow, button);
}

bool Input::MouseButtonPressed(Button button)
{
	int buttonCode = static_cast<int>(button);
	return MouseButtonPressed(buttonCode);
}

bool Input::MouseButtonReleased(Button button)
{
	int buttonCode = static_cast<int>(button);
	return MouseButtonReleased(buttonCode);
}

bool Input::MouseButtonDown(Button button)
{
	int buttonCode = static_cast<int>(button);
	return MouseButtonDown(buttonCode);
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

bool Input::KeyPressed(Key key)
{
	int keyCode = static_cast<int>(key);
	return KeyPressed(keyCode);
}

bool Input::KeyReleased(Key key)
{
	int keyCode = static_cast<int>(key);
	return KeyReleased(keyCode);
}

bool Input::KeyDown(Key key)
{
	int keyCode = static_cast<int>(key);
	return KeyDown(keyCode);
}

}