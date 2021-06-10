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

	debugRenderer = CreateRef<DebugRenderer>();
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

	debugRenderer->ClearLines();
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

static void CheckSideBlock(
	Vec2 charBlock, 
	Ref<DebugRenderer>& debugRenderer, 
	Ref<Character>& character, 
	const blocks_t& blocks, 
	int blockIndexX, 
	int blockIndexY,
	Vec2 affect,
	int checkX,
	int checkY,
	Vec2 characterSize,
	Vec2 correction,
	bool& variable,
	int times,
	bool reverse
) {
	for (int i = 0; i < times; ++i) {
		auto leftBlock = charBlock + Vec2(checkX, checkY) * 16.0f + affect * static_cast<float>(i) * 16.0f;
		float leaveSpace = 8.0f;
		debugRenderer->AddQuad(leftBlock, leftBlock + 16.0f);
		bool cond = reverse ? ( character->GetPosition().x + leaveSpace + characterSize.x < leftBlock.x + 16.0f) : (character->GetPosition().x + leaveSpace + characterSize.x > leftBlock.x + 16.0f);
		if (!(cond && blocks[blockIndexX + checkX + affect.x * i][blockIndexY + checkY + affect.y * i] != BlockType::Empty)) {
			variable = true;
		} else {
			if (correction.x) {
				character->SetPosition((leftBlock + 16.0f - leaveSpace + correction.x - characterSize.x).x, character->GetPosition().y);
			} else if (correction.y) {
				// character->SetPosition((leftBlock + 16.0f - leaveSpace + correction - characterWidth).x, character->GetPosition().y);
				character->SetPosition(character->GetPosition().x, (leftBlock + 16.0f - leaveSpace + correction.y - characterSize.y).y);
			}
			variable = false;
			break;
		}
	}
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

	debugRenderer->AddQuad(character->GetPosition() + Vec2(8.0f, 0.0f), character->GetPosition() + Vec2(3 * 16.0f, 4 * 16.0f) - Vec2(8.0f, 0.0f));

	std::vector<Pos> additionalLightData;

	//---------------------------------	
	const auto& blocks = world->GetMap()->GetBlocks();
	const auto& walls = world->GetMap()->GetWalls();

	const Pos blockPos = world->GetMap()->WindowCoordsToBlockCoords(Window::GetSize() / 2.0f - (camera->GetPosition() - character->GetPosition()) * Vec2(1, -1), Window::GetSpace(), camera->GetViewMatrix());

	for (int x = -1; x < 1; x++) {
		for (int y = -1; y < 2; y++) {
			if (blocks[blockPos.x + x][blockPos.y + y] == BlockType::Empty && walls[blockPos.x + x][blockPos.y + y] == BlockType::Empty) {
				additionalLightData.push_back(character->GetPosition() + Vec2(0, 64));
			}
		}
	}

	int blockIndexX = blockPos.x;
	int blockIndexY = blockPos.y;
	auto charBlock = (Vec2(blockIndexX, blockIndexY) - 8.0f) * 16.0f;

	CheckSideBlock(
		charBlock, 
		debugRenderer, 
		character, 
		blocks, 
		blockIndexX, 
		blockIndexY, 
		Vec2(0, 1), 
		0, 1,
		Vec2(0),
		Vec2(-1.0f, 0.0f),
		character->canMoveLeft,
		4,
		true
	);

	CheckSideBlock(
		charBlock, 
		debugRenderer, 
		character, 
		blocks, 
		blockIndexX, 
		blockIndexY, 
		Vec2(0, 1), 
		3, 1,
		Vec2(16.0f * 3.0f, 0.0f),
		Vec2(1.0f, 0.0f),
		character->canMoveRight,
		4,
		false
	);

	bool falls = true;

	CheckSideBlock(
		charBlock, 
		debugRenderer, 
		character, 
		blocks, 
		blockIndexX, 
		blockIndexY, 
		Vec2(1, 0), 
		1, 0,
		Vec2(0),
		Vec2(0.0f, 4.0f),
		falls,
		2,
		true
	);

	character->onGround = !falls;

	worldRenderer->Render([&]() {
		characterRenderer->Render({ character }, camera);
	}, additionalLightData);

	debugRenderer->Render(camera->GetViewMatrix());

	ImGui::Begin("Image");
		ImGui::Image((void*)(intptr_t)worldRenderer->mapPipeline.color->GetMapRenderer()->chunks[5][12].GetTargetTexture()->GetHandle(), ImVec2(16 * 16, 16 * 16), ImVec2(0, 0), ImVec2(1, -1));
	ImGui::End();

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(worldRenderer->mapPipeline.color->info.chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Lights: " + std::to_string(worldRenderer->mapPipeline.color->GetLightData().size())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		auto visibleChunks = world->GetMap()->GetVisibleChunks();
		ImGui::Text(("Position: " + std::to_string(camera->GetPosition().x) + ' ' + std::to_string(camera->GetPosition().y)).c_str());
		ImGui::Text(("Char Position: " + std::to_string(blockPos.x) + ' ' + std::to_string(blockPos.y)).c_str());
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