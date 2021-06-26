#include "Game.h"

#include "Linow/Linow.h"

#include "Core/Input/Input.h"
#include "Core/Time.h"

Game::Game() {
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

void Game::Play() {
  Logic();
  Render();
}

void Game::Logic() {
	if (Input::KeyPressed(Key::Space)) {
		if (character->rigidbody->GetOnGround()) {
			character->rigidbody->Jump();
		}
	}

	static float speed = 150.0f;

	if (Input::KeyDown(Key::A) && character->rigidbody->CanMoveLeft()) {
		character->SetPosition(character->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * speed);
		character->animator->SetFrame(character->animator->GetFrame() - 0.2f * Time::GetDelta() * speed);
		character->animator->SetDirection(-1);
	} else if (Input::KeyDown(Key::D) && character->rigidbody->CanMoveRight()) {
		character->SetPosition(character->GetPosition() + Vec2(1, 0) * Time::GetDelta() * speed);
		character->animator->SetFrame(character->animator->GetFrame() + 0.2f * Time::GetDelta() * speed);
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
}