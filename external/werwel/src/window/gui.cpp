#include "window/gui.h"

#include "window/window.h"

// #include "themes/theme0.h"
// #include "themes/theme1.h"

#include "window/input.h"

namespace ww {

void Gui::Create()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.IniFilename = nullptr;
	io.ConfigFlags = ImGuiConfigFlags_DockingEnable;
	defaultFont = io.Fonts->AddFontFromFileTTF(std::string("assets/fonts/Roboto-400.ttf").c_str(), 16);
	titleFont = io.Fonts->AddFontFromFileTTF(std::string("assets/fonts/Roboto-400.ttf").c_str(), 48);

	// Theme0();

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