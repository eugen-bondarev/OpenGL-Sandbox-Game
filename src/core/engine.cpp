#include "engine.h"

#include "core/input/input.h"
#include "core/window.h"
#include "core/time.h"
#include "core/gui.h"

#include "mgcwel/werwel.h"

Engine::Engine()
{
	ww::Window::Create({
		{ 1920, 1080 },
		ww::Window::Mode::Borderless,
		true
	});

	ww::Gui::Create();
	ww::Input::Create(ww::Window::GetGlfwWindow());
	Linow::Init();

	mainMenu = CreateRef<MainMenu>(game);
}

bool Engine::IsRunning() const
{
	return !ww::Window::ShouldClose();
}

void Engine::BeginFrame()
{
	ww::Window::BeginFrame();
	ww::Gui::BeginFrame();
	ww::Time::BeginFrame();
	ww::Input::BeginFrame();

	Linow::Clear();
}

void Engine::Run()
{
	if (resetGame)
	{
		game.reset();
		resetGame = false;
	}

	if (game)
	{
		game->Play(resetGame);
	}
	else
	{
		ww::GraphicsContext::Clear();
		mainMenu->Show();
	}

#ifdef NF_SHOW_DEBUG_GUI
	ImGui::SetNextWindowSize(ImVec2(140, 120));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text(("FPS: " + std::to_string(ww::Time::GetFps())).c_str());
	ImGui::End();
#endif
}

void Engine::EndFrame()
{
	ww::Input::EndFrame();
	ww::Time::EndFrame();
	ww::Gui::EndFrame();
	ww::Window::EndFrame();
}

Engine::~Engine()
{
	ww::Gui::Destroy();
	ww::Window::Destroy();
	ww::Window::Shutdown();
}