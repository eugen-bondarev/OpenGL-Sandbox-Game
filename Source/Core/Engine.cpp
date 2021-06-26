#include "Engine.h"

#include "Core/Input/Input.h"
#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"

#include "Renderer/Entities/RectVao.h"

#include "Werwel/GraphicsContext.h"

Engine::Engine() {
	Window::Create({ 1920, 1080 }, Window::Mode::Borderless, true, false);
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Primitives::Rect::Create();

	Linow::Init();
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::BeginFrame();
	Gui::BeginFrame();
	Time::BeginFrame();
	Input::BeginFrame();

	Linow::Clear();
}

void Engine::Render() {
	if (game) {
		game->Play();
	} else {
		Werwel::GraphicsContext::Clear();
	}

	ImGui::SetNextWindowSize(ImVec2(140, 120));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize);
		ImGui::Text(("FPS: " + std::to_string(Time::GetFps())).c_str());

		if (ImGui::Button("Start game")) {
			game = CreatePtr<Game>();
		}

		if (ImGui::Button("Stop game")) {
			game.reset();
		}
	ImGui::End();
}

void Engine::EndFrame() {
	Input::EndFrame();
	Time::EndFrame();
	Gui::EndFrame();
	Window::EndFrame();
}

Engine::~Engine() {
	Gui::Destroy();
	Window::Destroy();
}