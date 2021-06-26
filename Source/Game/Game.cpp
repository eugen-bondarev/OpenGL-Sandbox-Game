#include "Game.h"

#include "Linow/Linow.h"

#include "Core/Input/Input.h"
#include "Core/Time.h"

#include "imgui/imgui.h"

Game::Game(int seed) {
	map = CreateRef<Map>(seed, Vec2(5, 5), Vec2(500, 500));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());
	map->CalculateVisibleChunks(camera->GetPosition());
	mapRenderer = CreateRef<MapRenderer>(map, camera);

	character = CreateRef<Character>(map);
	character->SetPosition(camera->GetPosition() + Vec2(0, 250.0f));
	
	character->GetComponent<Rigidbody>()->Update();

	bool posChanged { false };

	while (character->GetComponent<Rigidbody>()->GetCeiling()) {
		character->SetPositionY(character->GetPosition().y + 16.0f);
		character->GetComponent<Rigidbody>()->Update();
		posChanged = true;
	}

	if (posChanged) {
		character->SetPositionY(character->GetPosition().y + 128.0f);
		character->GetComponent<Rigidbody>()->Update();
	}

	while (!character->GetComponent<Rigidbody>()->GetOnGround()) {
		character->GetComponent<Rigidbody>()->Update();
	}

	characters.push_back(character);

	characterRenderer = CreateRef<CharacterRenderer>(characters, camera);
}

void Game::Play(bool& resetGame) {
  Logic(resetGame);
  Render();
}

void Game::Logic(bool& resetGame) {
	if (Input::KeyPressed(Key::Esc)) {
		resetGame = true;
	}

	if (Input::KeyPressed(Key::Space)) {
		if (character->rigidbody->GetOnGround()) {
			character->rigidbody->Jump();
		}
	}

	static float defaultSpeed = 150.0f;

	if (Input::KeyDown(Key::A) && character->rigidbody->CanMoveLeft()) {
		character->SetPosition(character->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * defaultSpeed);
		character->animator->SetFrame(character->animator->GetFrame() - 0.2f * Time::GetDelta() * defaultSpeed);
		character->animator->SetDirection(-1);
	} else if (Input::KeyDown(Key::D) && character->rigidbody->CanMoveRight()) {
		character->SetPosition(character->GetPosition() + Vec2(1, 0) * Time::GetDelta() * defaultSpeed);
		character->animator->SetFrame(character->animator->GetFrame() + 0.2f * Time::GetDelta() * defaultSpeed);
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

void Game::Render() {
	camera->OnPositionChange([&]() {
		map->CalculateVisibleChunks(camera->GetPosition());
		mapRenderer->rerender = true;
	});

	character->rigidbody->Update();
	camera->SetPosition(character->GetPosition());
	character->CollectLights(mapRenderer->GetAdditionalLightData());

	mapRenderer->Render({ characterRenderer });

	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(camera->GetTransform()));

	ImGui::SetNextWindowSize(ImVec2(220, 120));
	ImGui::SetNextWindowPos(ImVec2(Window::GetPosition().x + 140 + 20 + 20, Window::GetPosition().y + 20));
	ImGui::Begin("Map info", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text(std::string("Area: " + std::to_string(map->GetWidth()) + 'x' + std::to_string(map->GetHeight()) + " = " + std::to_string(map->GetArea())).c_str());
    ImGui::Text(std::string("Size (b): " + std::to_string(map->GetSizeInBytes())).c_str());
    ImGui::Text(std::string("Size (kb): " + std::to_string(map->GetSizeInKilobytes())).c_str());
    ImGui::Text(std::string("Size (mb): " + std::to_string(map->GetSizeInMegabytes())).c_str());
	ImGui::End();
}