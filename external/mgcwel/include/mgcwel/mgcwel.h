#pragma once

#include "fbo.h"
#include "graphics_context.h"
#include "shader.h"
#include "texture.h"
#include "vao.h"
#include "vbo.h"
#include "vertex_buffer_layout.h"

#include "assets/image_asset.h"
#include "assets/text_asset.h"

#include "window/window.h"
#include "window/input.h"
#include "window/time.h"
#include "window/gui.h"

#include "maths/maths.h"

#define MW_WINDOW_WIDTH() 			::mw::Window::GetSize().x
#define MW_WINDOW_HEIGHT() 			::mw::Window::GetSize().y

#define MW_KEY_PRESSED(KEY) 		::mw::Input::KeyPressed(KEY)
#define MW_KEY_RELEASED(KEY) 		::mw::Input::KeyReleased(KEY)
#define MW_KEY_DOWN(KEY) 			::mw::Input::KeyDown(KEY)

#define MW_MOUSE_PRESSED(KEY) 		::mw::Input::MouseButtonPressed(KEY)
#define MW_MOUSE_RELEASED(KEY) 		::mw::Input::MouseButtonReleased(KEY)
#define MW_MOUSE_DOWN(KEY) 			::mw::Input::MouseButtonDown(KEY)