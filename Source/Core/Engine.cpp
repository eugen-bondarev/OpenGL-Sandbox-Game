#include "Engine.h"

#include "Core/Input/Input.h"
#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"

#include "Werwel/GraphicsContext.h"

Engine::Engine()
{
	Window::Create({
		{ 1920, 1080 },
		Window::Mode::Borderless,
		true
	});

	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Linow::Init();

	mainMenu = CreateRef<MainMenu>(game);
}

bool Engine::IsRunning() const
{
	return !Window::ShouldClose();
}

void Engine::BeginFrame()
{
	Window::BeginFrame();
	Gui::BeginFrame();
	Time::BeginFrame();
	Input::BeginFrame();

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
		Werwel::GraphicsContext::Clear();
		mainMenu->Show();
	}

#ifdef NF_SHOW_DEBUG_GUI
	ImGui::SetNextWindowSize(ImVec2(140, 120));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text(("FPS: " + std::to_string(Time::GetFps())).c_str());
	ImGui::End();
#endif
}

void Engine::EndFrame()
{
	Input::EndFrame();
	Time::EndFrame();
	Gui::EndFrame();
	Window::EndFrame();
}

Engine::~Engine()
{
	Gui::Destroy();
	Window::Destroy();
	Window::Shutdown();
}