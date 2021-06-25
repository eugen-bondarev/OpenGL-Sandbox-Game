#include "Engine.h"

#include "Core/Input/Input.h"
#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"

#include "Renderer/Entities/RectVao.h"

#include "Werwel/GraphicsContext.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Primitives::Rect::Create();

	Linow::Init();
}

void Engine::InitResources() {
	NATURAFORGE_PROFILER_SCOPE();
	map = CreateRef<Map>(Vec2(5, 5), Vec2(500, 500));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());
	map->CalculateVisibleChunks(camera->GetPosition());
	mapRenderer = CreateRef<MapRenderer>(map, camera);

	character = CreateRef<Character>(map);
	character->SetPosition(camera->GetPosition() + Vec2(0, 250.0f));

	characters.push_back(character);

	characterRenderer = CreateRef<CharacterRenderer>(characters, camera);
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
	if (Input::KeyPressed(Key::Space)) {
		if (character->rigidbody->GetOnGround()) {
			character->rigidbody->Jump();
		}
	}

	static float speed = 1500.0f;

	if (Input::KeyDown(Key::A) && character->rigidbody->CanMoveLeft()) {
		character->SetPosition(character->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * speed);
		character->animator->SetFrame(character->animator->GetFrame() - 0.3f * Time::GetDelta() * speed);
		character->animator->SetDirection(-1);
	} else if (Input::KeyDown(Key::D) && character->rigidbody->CanMoveRight()) {
		character->SetPosition(character->GetPosition() + Vec2(1, 0) * Time::GetDelta() * speed);
		character->animator->SetFrame(character->animator->GetFrame() + 0.3f * Time::GetDelta() * speed);
		character->animator->SetDirection(1);
	} else {
		character->animator->SetFrame(character->animator->GetDirection() == 1 ? 0 : -1);
	}

	if (Input::MouseButtonDown(Button::Left)) {
		Map::BlockSettingData settingBlock = map->PlaceBlock(camera->GetPosition(), BlockType::Empty);

		if (settingBlock.IsSet()) {
			mapRenderer->rerender = true;
			mapRenderer->chunksUpdated = true;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		Map::BlockSettingData settingBlock = map->PlaceBlock(camera->GetPosition(), BlockType::Dirt);

		if (settingBlock.IsSet()) {
			mapRenderer->rerender = true;
			mapRenderer->chunksUpdated = true;
		}
	}
}

void Engine::Render() {
	camera->OnPositionChange([&]() {
		map->CalculateVisibleChunks(camera->GetPosition());
		mapRenderer->rerender = true;
	});

	character->rigidbody->Update();
	camera->SetPosition(character->GetPosition());
	character->CollectLights(mapRenderer->GetAdditionalLightData());

	mapRenderer->Render({ characterRenderer });

	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(camera->GetTransform()));

	ImGui::Begin("Info");
		ImGui::Text(("FPS:" + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Blocks:" + std::to_string(mapRenderer->GetAmountOfRenderedBlocks())).c_str());
		ImGui::Text(("Walls:" + std::to_string(mapRenderer->GetAmountOfRenderedWalls())).c_str());
		ImGui::Text(("Lights:" + std::to_string(mapRenderer->GetAmountOfRenderedLights())).c_str());
		ImGui::Text(std::to_string(map->GetVisibleChunks().x.start).c_str());
		ImGui::Text(std::to_string(map->GetVisibleChunks().x.end).c_str());
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