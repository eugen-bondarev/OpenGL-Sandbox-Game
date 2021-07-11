#include "game.h"

#include "Linow/Linow.h"

#include "imgui/imgui.h"

Game::Game(int seed)
{
	const mw::ImageAsset toolsTileMapTexture("assets/images/tools.png");
	toolsTileMap = TextureAtlas::Add<ToolsTileMap>(TextureAtlasType::Tools, CreateRef<ToolsTileMap>(
		Vec2(16.0f),
		toolsTileMapTexture.GetSize(),
		toolsTileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		mw::Texture::Parameters_t{
			mw::Texture::SetInterpolation(mw::Interpolation::Constant)}));

	toolsTileMap->Add(ToolType::BronzePickaxe, Vec2(0, 0));
	toolsTileMap->Add(ToolType::BronzeAxe, Vec2(1, 0));
	toolsTileMap->Add(ToolType::BronzeShovel, Vec2(2, 0));

	const mw::ImageAsset interfaceIconsTileMapAsset("assets/images/icons/icons1.png");
	interfaceIconsTileMap = TextureAtlas::Add<InterfaceIconTileMap>(TextureAtlasType::InterfaceIcons, CreateRef<InterfaceIconTileMap>(
		Vec2(16.0f),
		interfaceIconsTileMapAsset.GetSize(),
		interfaceIconsTileMapAsset.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		mw::Texture::Parameters_t{
			mw::Texture::SetInterpolation(mw::Interpolation::Constant)}));

	interfaceIconsTileMap->Add(InterfaceIconType::Bag, Vec2(0, 0));

	world = CreateRef<World>(seed);
	worldRenderer = CreateRef<WorldRenderer>(world);

	character = CreateRef<Character>(world);
	characterRenderer = CreateRef<CharacterRenderer>(characters, world->GetCamera());
	characters.push_back(character);

	interfaceRenderer = CreateRef<InterfaceRenderer>(character->GetComponent<Player>()->GetInventory());
}

Game::~Game()
{
	IUpdatable::updatableComponents.clear();
	TextureAtlas::Clear();
}

void Game::Play(bool &resetGame)
{
	Logic(resetGame);
	Render();
}

void Game::Logic(bool &resetGame)
{
	if (mw::Input::KeyPressed(mw::Input::Key::Esc))
	{
		resetGame = true;
	}
}

void Game::Render()
{
	IUpdatable::UpdateComponents();

	world->GetCamera()->SetPosition(character->GetPosition() + Vec2(24, 24));
	character->CollectLights(worldRenderer->GetMapRenderer()->GetAdditionalLightData());

	for (int i = 0; i < worldRenderer->GetWoodsRenderer()->visibleTrees.size(); i++)
	{
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

	worldRenderer->Render(
	{
		worldRenderer->GetWoodsRenderer(), 
		characterRenderer
	});

	interfaceRenderer->Render();

	ImGui::SetNextWindowPos(ImVec2(20, 160));
	ImGui::SetNextWindowSize(ImVec2(200, 160));
	ImGui::Begin("Map settings", nullptr, ImGuiWindowFlags_NoResize);
		ImGui::DragFloat("Size 0", &MapSettings::SIZE_0, 0.001f, 0.00001f, 32.0f);
		ImGui::DragFloat("Size 1", &MapSettings::SIZE_1, 0.001f, 0.00001f, 32.0f);
		ImGui::DragFloat("Size 2", &MapSettings::SIZE_2, 0.001f, 0.00001f, 32.0f);
		ImGui::DragFloat("Bias 0", &MapSettings::BIAS_0, 0.001f, -1.0f, 1.0f);
		ImGui::DragFloat("Bias 1", &MapSettings::BIAS_1, 0.001f, -1.0f, 1.0f);
	ImGui::End();

	Linow::Render(Math::ToPtr(mw::Window::GetSpace()), Math::ToPtr(world->GetCamera()->GetTransform()));

#ifdef NF_SHOW_DEBUG_GUI
	ImGui::SetNextWindowSize(ImVec2(120, 120));
	ImGui::SetNextWindowPos(ImVec2(140 + 20 + 20, 20));
	ImGui::Begin("Map info", nullptr, ImGuiWindowFlags_NoResize);
	ImGui::Text(std::string("Blocks: " + std::to_string(worldRenderer->GetMapRenderer()->sortedBlocks.size())).c_str());
	ImGui::Text(std::string("Lights: " + std::to_string(worldRenderer->GetMapRenderer()->sortedLights.size())).c_str());
	ImGui::Text(std::string("Walls: " + std::to_string(worldRenderer->GetMapRenderer()->sortedWalls.size())).c_str());
	ImGui::End();
#endif
}