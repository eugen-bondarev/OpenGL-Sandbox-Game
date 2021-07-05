#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

namespace ww {

class Gui
{
public:
	static void Create();
	static void Destroy();

	static void BeginFrame();
	static void EndFrame();

	inline static ImFont *defaultFont{nullptr};
	inline static ImFont *titleFont{nullptr};
};

}