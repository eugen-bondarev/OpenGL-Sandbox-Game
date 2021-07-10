#include "engine.h"

#include "mgcwel/mgcwel.h"

#include "FastNoise/FastNoiseLite.h"

mw::Texture* noiseTexture { nullptr };

FastNoiseLite noise;

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

	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noiseTexture = new mw::Texture(Vec2(512), nullptr, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, mw::Texture::Parameters_t { mw::Texture::SetInterpolation(mw::Interpolation::Constant) });
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
#ifdef NF_RENDER_GAME
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

#else
	mw::GraphicsContext::Clear();

	int j = 0;

	Vec2 textureSize = Vec2(8);
	static float offsetX = 0.0f;
	static float offsetY = 0.0f;
	static float size = 32.0f;
	static float octaves = 5.0f;
	static Vec2i chunk;

	std::vector<unsigned char> textureData(textureSize.x * textureSize.y * 3);
	for (int x = 0; x < textureSize.x; x++)
	{
		for (int y = 0; y < textureSize.y; y++)
		{
			float value = noise.GetNoise(static_cast<float>(x + chunk.x) * size, static_cast<float>(y + chunk.y) * size);

			for (int i = 0; i < 3; i++)
			{
				textureData[j++] = (value * 0.5f + 0.5f) * 255;
			}
		}
	}
	delete noiseTexture;
	noiseTexture = new mw::Texture(
		textureSize, 
		textureData.data(), 
		GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, 
		mw::Texture::Parameters_t { mw::Texture::SetInterpolation(mw::Interpolation::Constant) }
	);

	ImGui::SetNextWindowSize(ImVec2(1200, 1080));
	ImGui::Begin("Nois1e");

		if (ImGui::Button("Regenerate"))
		{
		}

		ImGui::DragInt2("Chunk", &chunk.x, 1.00f, 0.0f, 256.0f);
		ImGui::DragFloat("Size", &size, 1.0f / 25.6f * 10.0f, 0, 1024);
		ImGui::DragFloat("Octaves", &octaves, 1.0f / 25.6f * 10.0f, 0, 256);

		static float step = 1.00f;
		static Vec2 shift;
		ImGui::DragFloat("step", &step, 0.01f, 0.0f, 256.0f);
		ImGui::DragFloat2("shift", &shift.x, 0.01f, 0.0f, 256.0f);

		if (ImGui::Button("Right")) 
		{
			offsetX += 1.0f / 25.6f * 10.0f;
		}

		ImGui::Image((void*)(intptr_t)noiseTexture->GetHandle(), ImVec2(512, 512), 
			ImVec2(0 + shift.x, 0 + shift.y),
			ImVec2(step + shift.x, step + shift.y)
		);
	ImGui::End();

	// ImGui::ShowDemoWindow();
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
	delete noiseTexture;

	mw::Gui::Destroy();
	mw::Window::Destroy();
	mw::Window::Shutdown();
}