#pragma once

#include "werwel/werwel.h"

#define NF_KEY_PRESSED(KEY) 		::ww::Input::KeyPressed(KEY)
#define NF_KEY_RELEASED(KEY) 		::ww::Input::KeyReleased(KEY)
#define NF_KEY_DOWN(KEY) 			::ww::Input::KeyDown(KEY)

#define NF_MOUSE_PRESSED(KEY) 		::ww::Input::MouseButtonPressed(KEY)
#define NF_MOUSE_RELEASED(KEY) 		::ww::Input::MouseButtonReleased(KEY)
#define NF_MOUSE_DOWN(KEY) 			::ww::Input::MouseButtonDown(KEY)