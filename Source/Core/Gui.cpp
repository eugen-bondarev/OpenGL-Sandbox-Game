#include "Gui.h"

#include "Window.h"

#include "Themes/Theme0.h"
#include "Themes/Theme1.h"

void Gui::Create() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = std::string(NATURAFORGE_ROOT + "imgui.ini").c_str();
    io.ConfigFlags = ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

		io.Fonts->AddFontFromFileTTF(std::string(NATURAFORGE_ROOT + "Assets/Fonts/Roboto-400.ttf").c_str(), 16);

	Theme0();

	ImGui_ImplGlfw_InitForOpenGL(Window::GetGlfwWindow(), true);
	ImGui_ImplOpenGL3_Init();
}

void Gui::Destroy() {
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();
}

void Gui::BeginFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Gui::EndFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		GLFWwindow* backupCurrentContext = glfwGetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();

		glfwMakeContextCurrent(backupCurrentContext);
	}
}