#include "engine.h"

#include "mgcwel/mgcwel.h"

Engine::Engine()
{
	mw::Window::Create({
		{ 1920, 1080 },
		mw::Window::Mode::Borderless,
		true
	});

	mw::Gui::Create();
	mw::Input::Create(mw::Window::GetGlfwWindow());
	Linow::Init();

	mainMenu = CreateRef<MainMenu>(game);
}

bool Engine::IsRunning() const
{
	return !mw::Window::ShouldClose();
}

void Engine::BeginFrame()
{
	mw::Window::BeginFrame();
	mw::Gui::BeginFrame();
	mw::Time::BeginFrame();
	mw::Input::BeginFrame();

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
		mw::GraphicsContext::Clear();
		mainMenu->Show();
	}

#ifdef NF_SHOW_DEBUG_GUI
	ImGui::SetNextWindowSize(ImVec2(140, 120));
	ImGui::SetNextWindowPos(ImVec2(20, 20));
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text(("FPS: " + std::to_string(mw::Time::GetFps())).c_str());
	ImGui::End();
#endif
}

void Engine::EndFrame()
{
	mw::Input::EndFrame();
	mw::Time::EndFrame();
	mw::Gui::EndFrame();
	mw::Window::EndFrame();
}

Engine::~Engine()
{
	mw::Gui::Destroy();
	mw::Window::Destroy();
	mw::Window::Shutdown();
}