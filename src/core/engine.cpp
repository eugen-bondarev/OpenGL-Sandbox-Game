#include "engine.h"

#include "mgcwel/mgcwel.h"

#include "PerlinNoise/PerlinNoise.hpp"

siv::PerlinNoise perlinA(std::random_device{});

mw::Texture* noiseTexture { nullptr };

Engine::Engine()
{
	mw::Window::Create({
		{ 1920, 1080 },
		mw::Window::Mode::Borderless,
		true
	});

	mw::Input::Create(mw::Window::GetGlfwWindow());
	mw::Gui::Create();
	Linow::Init();

	mainMenu = CreateRef<MainMenu>(game);

	std::vector<unsigned char> data(512 * 512 * 3);

	for (int x = 0; x < 512; x++)
	{
		
	}

	noiseTexture = new mw::Texture(Vec2(512), data.data(), GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, mw::Texture::Parameters_t {});
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
// #ifdef NF_RENDER_GAME
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

// #else
// 	mw::GraphicsContext::Clear();

// 	ImGui::Image((void*)(intptr_t)noiseTexture->GetHandle(), ImVec2(512, 512));

// 	// ImGui::ShowDemoWindow();
// #endif
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
	delete noiseTexture;

	mw::Gui::Destroy();
	mw::Window::Destroy();
	mw::Window::Shutdown();
}