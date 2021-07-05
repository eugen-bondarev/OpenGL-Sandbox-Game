#include "Game.h"

#include "Linow/Linow.h"

#include "Core/Input/Input.h"
#include "Core/Time.h"

#include "imgui/imgui.h"

Game::Game(int seed) {
	const ImageAsset toolsTileMapTexture("Assets/Images/Tools.png");
	toolsTileMap = TextureAtlas::Add<ToolsTileMap>(TextureAtlasType::Tools, CreateRef<ToolsTileMap>(
		Vec2(16.0f),
		toolsTileMapTexture.GetSize(),
		toolsTileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		Werwel::Texture::Parameters_t {
			Werwel::Texture::SetInterpolation(Werwel::Interpolation::Constant)
		}
	));
	
	toolsTileMap->Add(ToolType::BronzePickaxe, Vec2(0, 0));
	toolsTileMap->Add(ToolType::BronzeAxe, Vec2(1, 0));
	toolsTileMap->Add(ToolType::BronzeShovel, Vec2(2, 0));

	world = CreateRef<World>(seed);
	worldRenderer = CreateRef<WorldRenderer>(world);

	character = CreateRef<Character>(world);
	characterRenderer = CreateRef<CharacterRenderer>(characters, world->GetCamera());
	characters.push_back(character);

	interfaceRenderer = CreateRef<InterfaceRenderer>(character->GetComponent<Player>()->GetInventory());

}

Game::~Game() {
	IUpdatable::updatableComponents.clear();
	TextureAtlas::Clear();
}

void Game::Play(bool& resetGame) {
  Logic(resetGame);
  Render();
}

void Game::Logic(bool& resetGame) {
	if (Input::KeyPressed(Input::Key::Esc)) {
		resetGame = true;
	}
}

void Game::Render() {
	IUpdatable::UpdateComponents();

	world->GetCamera()->SetPosition(character->GetPosition() + Vec2(24, 24));
	character->CollectLights(worldRenderer->GetMapRenderer()->GetAdditionalLightData());

	for (int i = 0; i < worldRenderer->GetWoodsRenderer()->visibleTrees.size(); i++) {
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(-64, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(-48, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(-32, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(-16, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(16, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(32, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(48, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(64, 80));
		
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(0, 80));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(0, 32));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(0, 0));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(0, -32));
		worldRenderer->GetMapRenderer()->GetAdditionalLightData().emplace_back(worldRenderer->GetWoodsRenderer()->visibleTrees[i] + Vec2(0, -80));
	}

	worldRenderer->Render({ worldRenderer->GetWoodsRenderer(), characterRenderer });

	interfaceRenderer->Render();

	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(world->GetCamera()->GetTransform()));

#ifdef NF_SHOW_DEBUG_GUI
	ImGui::SetNextWindowSize(ImVec2(220, 120));
	ImGui::SetNextWindowPos(ImVec2(140 + 20 + 20, 20));
	ImGui::Begin("Map info", nullptr, ImGuiWindowFlags_NoResize);
    ImGui::Text(std::string("Blocks: " + std::to_string(worldRenderer->GetMapRenderer()->GetAmountOfRenderedBlocks())).c_str());
    ImGui::Text(std::string("Walls: " + std::to_string(worldRenderer->GetMapRenderer()->GetAmountOfRenderedWalls())).c_str());
    ImGui::Text(std::string("Area: " + std::to_string(world->GetMap()->GetWidth()) + 'x' + std::to_string(world->GetMap()->GetHeight()) + " = " + std::to_string(world->GetMap()->GetArea())).c_str());
    ImGui::Text(std::string("Size (mb): " + std::to_string(world->GetMap()->GetSizeInMegabytes())).c_str());
	ImGui::End();
#endif
}