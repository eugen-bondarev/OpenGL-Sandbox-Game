#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class Gui {
public:
	static void Create();
	static void Destroy();

	static void Begin();
	static void End();
};