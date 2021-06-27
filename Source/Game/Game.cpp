#include "Game.h"

#include "Linow/Linow.h"

#include "Core/Input/Input.h"
#include "Core/Time.h"

#include "imgui/imgui.h"

Game::Game(int seed) {
	world = CreateRef<World>(669);
	worldRenderer = CreateRef<WorldRenderer>(world);

	character = CreateRef<Character>(world);
	characterRenderer = CreateRef<CharacterRenderer>(characters, world->GetCamera());
	characters.push_back(character);

	interfaceRenderer = CreateRef<InterfaceRenderer>(character->GetComponent<Player>()->GetInventory());
}

void Game::Play(bool& resetGame) {
  Logic(resetGame);
  Render();
}

void Game::Logic(bool& resetGame) {
	if (Input::KeyPressed(Key::Esc)) {
		resetGame = true;
	}
}

void Game::Render() {
	Component::UpdateComponents();

	world->GetCamera()->SetPosition(character->GetPosition());
	character->CollectLights(worldRenderer->GetMapRenderer()->GetAdditionalLightData());

	worldRenderer->Render({ characterRenderer });

	interfaceRenderer->Render();

	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(world->GetCamera()->GetTransform()));

	ImGui::SetNextWindowSize(ImVec2(220, 120));
	ImGui::SetNextWindowPos(ImVec2(Window::GetPosition().x + 140 + 20 + 20, Window::GetPosition().y + 20));
	ImGui::Begin("Map info", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text(std::string("Area: " + std::to_string(world->GetMap()->GetWidth()) + 'x' + std::to_string(world->GetMap()->GetHeight()) + " = " + std::to_string(world->GetMap()->GetArea())).c_str());
    ImGui::Text(std::string("Size (b): " + std::to_string(world->GetMap()->GetSizeInBytes())).c_str());
    ImGui::Text(std::string("Size (kb): " + std::to_string(world->GetMap()->GetSizeInKilobytes())).c_str());
    ImGui::Text(std::string("Size (mb): " + std::to_string(world->GetMap()->GetSizeInMegabytes())).c_str());
	ImGui::End();
}