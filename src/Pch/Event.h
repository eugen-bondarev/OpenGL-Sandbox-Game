#pragma once

#include "Core/Window.h"
#include "Core/Input/Input.h"

#define NF_KEY_PRESSED(KEY) 		::Input::KeyPressed(KEY)
#define NF_KEY_RELEASED(KEY) 		::Input::KeyReleased(KEY)
#define NF_KEY_DOWN(KEY) 				::Input::KeyDown(KEY)

#define NF_MOUSE_PRESSED(KEY) 	::Input::MouseButtonPressed(KEY)
#define NF_MOUSE_RELEASED(KEY) 	::Input::MouseButtonReleased(KEY)
#define NF_MOUSE_DOWN(KEY) 			::Input::MouseButtonDown(KEY)