#pragma once

namespace {

enum class State
{
	Idle = 0,
	Pressed,
	Released
};

}

using KeyState = State;
using ButtonState = State;

enum class MouseWheelState
{
	Idle = 0,
	Up,
	Down
};