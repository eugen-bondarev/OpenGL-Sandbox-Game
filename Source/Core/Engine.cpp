#include "Engine.h"

#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"
#include "Core/Input/Input.h"

#include "Renderer/Entities/RectVao.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Util/ImGuiHelper.h"

#include "Werwel/GraphicsContext.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Primitives::Rect::Create();

	Linow::Init(Math::ToPtr(Window::GetSpace()));
}

void Engine::InitResources() {
	FORGIO_PROFILER_SCOPE();

	map = CreateRef<Map>(Size(5, 5), Size(250, 250));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());

	map->CalculateVisibleChunks(camera->GetPosition());

	mapRenderer = CreateRef<MapRenderer>(map);
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

void Engine::Control() {
	if (Input::KeyDown(Key::W)) {
		camera->SetPosition(camera->GetPosition() + Vec2(0, 1) * Time::GetDelta() * 300.0f);
	}

	if (Input::KeyDown(Key::A)) {
		camera->SetPosition(camera->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * 300.0f);
	}

	if (Input::KeyDown(Key::S)) {
		camera->SetPosition(camera->GetPosition() + Vec2(0, -1) * Time::GetDelta() * 300.0f);
	}

	if (Input::KeyDown(Key::D)) {
		camera->SetPosition(camera->GetPosition() + Vec2(1, 0) * Time::GetDelta() * 300.0f);
	}

	if (Input::MouseButtonDown(Button::Left)) {
		BlockSettingData& settingBlock = map->SetBlock(camera->GetPosition(), BlockType::Empty);

		if (settingBlock.IsSet()) {
			mapRenderer->RerenderChunk(settingBlock.chunk.x, settingBlock.chunk.y);
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		BlockSettingData& settingBlock = map->SetBlock(camera->GetPosition(), BlockType::Dirt);

		if (settingBlock.IsSet()) {
			mapRenderer->RerenderChunk(settingBlock.chunk.x, settingBlock.chunk.y);
		}
	}
}

void Engine::Render() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	camera->OnPositionChange([&]() {
		map->CalculateVisibleChunks(camera->GetPosition());
		mapRenderer->OnVisibleChunksChange();
	});
	
	mapRenderer->Render(camera);
	
	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(camera->GetViewMatrix()));

	ImGui::Begin("Info");
		ImGui::Text(("FPS:" + std::to_string(Time::GetFps())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		ImGui::Text(("x: " + std::to_string(mapRenderer->GetVisibleChunks().x.start) + " " + std::to_string(mapRenderer->GetVisibleChunks().x.end)).c_str());
		ImGui::Text(("y: " + std::to_string(mapRenderer->GetVisibleChunks().y.start) + " " + std::to_string(mapRenderer->GetVisibleChunks().y.end)).c_str());
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