#include "window/gui.h"

#include "window/window.h"

#include "themes/theme_0.h"
#include "themes/theme_1.h"
#include "themes/theme_2.h"
#include "themes/theme_3.h"
#include "themes/theme_4.h"
#include "themes/theme_5.h"

#include "window/input.h"

namespace mw {

void Gui::Create()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
	defaultFont = io.Fonts->AddFontFromFileTTF(std::string(ROOT_DIR + "assets/fonts/OtomanopeeOne-Regular.ttf").c_str(), 18);
	titleFont = io.Fonts->AddFontFromFileTTF(std::string(ROOT_DIR + "assets/fonts/OtomanopeeOne-Regular.ttf").c_str(), 48);

	Theme5();

	ImGui_ImplGlfw_InitForOpenGL(Window::GetGlfwWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

void Gui::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void Gui::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	// ImGui::ShowDemoWindow();
}

void Gui::EndFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO &io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow *backupCurrentContext = glfwGetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		glfwMakeContextCurrent(backupCurrentContext);
	}
}

}