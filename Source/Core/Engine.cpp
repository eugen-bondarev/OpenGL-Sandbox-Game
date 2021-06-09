#include "Engine.h"

#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"
#include "Core/Input/Input.h"

#include "Renderer/Entities/RectVao.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Primitives::Rect::Create();
}

void Engine::InitResources() {
	FORGIO_PROFILER_SCOPE();

	camera = CreateRef<Camera>();	
	world = CreateRef<World>();
	worldRenderer = CreateRef<WorldRenderer>(world, camera);

	character = CreateRef<Character>();
	character->SetPosition(camera->GetPosition());
	characterRenderer = CreateRef<CharacterRenderer>();
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::BeginFrame();
	Gui::BeginFrame();
	Time::BeginFrame();
	Input::BeginFrame();
}

void Engine::Control() {
	if (Input::KeyDown(Key::A)) {
		if (character->canMoveLeft) {
			character->AddPosition(Vec2(-1,  0) * Time::GetDelta() * 150.0f);
		}
	}

	if (Input::KeyDown(Key::D)) {
		if (character->canMoveRight) {
			character->AddPosition(Vec2( 1,  0) * Time::GetDelta() * 150.0f);
		} 
	}

	if (Input::KeyDown(Key::Space)) {
		if (character->onGround) {
			character->Jump();
		}
	}

	camera->SetPosition(character->GetPosition());
}

void Engine::Render() {
	if (Input::MouseButtonDown(Button::Left)) {
		auto& settingBlock = world->GetMap()->SetBlock(camera->GetViewMatrix(), BlockType::Empty);
		if (settingBlock.IsSet()) {
			worldRenderer->mapPipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			worldRenderer->mapPipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);
			worldRenderer->ForceRerender();
			lastDestroyedBlock = settingBlock.block;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		auto& settingBlock = world->GetMap()->SetBlock(camera->GetViewMatrix(), BlockType::Dirt);
		if (settingBlock.IsSet()) {
			worldRenderer->mapPipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			worldRenderer->mapPipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);
			worldRenderer->ForceRerender();
		}
	}

	character->Update(Time::GetDelta());

	std::vector<Pos> additionalLightData;

	//---------------------------------	
	const auto& blocks = world->GetMap()->GetBlocks();
	const auto& walls = world->GetMap()->GetWalls();
	Pos blockNearPlayer = Vec2(character->GetPosition().x, character->GetPosition().y) / world->GetMap()->GetBlockSize() + world->GetMap()->GetChunkSize() / 2.0f;

	character->canMoveLeft = !(blocks[blockNearPlayer.x - 1][blockNearPlayer.y + 1] != BlockType::Empty || blocks[blockNearPlayer.x - 1][blockNearPlayer.y + 2] != BlockType::Empty || blocks[blockNearPlayer.x - 1][blockNearPlayer.y + 3] != BlockType::Empty || blocks[blockNearPlayer.x - 1][blockNearPlayer.y + 4] != BlockType::Empty);
	character->canMoveRight = !(blocks[blockNearPlayer.x + 1][blockNearPlayer.y + 1] != BlockType::Empty || blocks[blockNearPlayer.x + 1][blockNearPlayer.y + 2] != BlockType::Empty || blocks[blockNearPlayer.x + 1][blockNearPlayer.y + 3] != BlockType::Empty || blocks[blockNearPlayer.x + 1][blockNearPlayer.y + 4] != BlockType::Empty);

	for (int x = -1; x < 1; x++) {
		for (int y = -1; y < 2; y++) {
			if (blocks[blockNearPlayer.x + x][blockNearPlayer.y + y] == BlockType::Empty && walls[blockNearPlayer.x + x][blockNearPlayer.y + y] == BlockType::Empty) {
				additionalLightData.push_back(character->GetPosition() + Vec2(0, 64));
			}
		}
	}

	if (blocks[blockNearPlayer.x][blockNearPlayer.y] == BlockType::Empty && blocks[blockNearPlayer.x + 1][blockNearPlayer.y] == BlockType::Empty) {
		character->onGround = false;
	} else {
		character->onGround = true;
	}
	//---------------------------------

	worldRenderer->Render([&]() {
		characterRenderer->Render({ character }, camera);
	}, additionalLightData);

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(worldRenderer->mapPipeline.color->info.chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Lights: " + std::to_string(worldRenderer->mapPipeline.color->GetLightData().size())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		auto visibleChunks = world->GetMap()->GetVisibleChunks();
		ImGui::Text(("Position: " + std::to_string(camera->GetPosition().x) + ' ' + std::to_string(camera->GetPosition().y)).c_str());
		ImGui::Text(("Char Position: " + std::to_string(blockNearPlayer.x) + ' ' + std::to_string(blockNearPlayer.y)).c_str());
		ImGui::Text(("Last d. block: " + std::to_string(lastDestroyedBlock.x) + ' ' + std::to_string(lastDestroyedBlock.y)).c_str());
		ImGui::Text(("Chunk x: " + std::to_string(visibleChunks.x.start) + ' ' + std::to_string(visibleChunks.x.end)).c_str());
		ImGui::Text(("Chunk y: " + std::to_string(visibleChunks.y.start) + ' ' + std::to_string(visibleChunks.y.end)).c_str());
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