#include "Engine.h"

#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"
#include "Core/Input/Input.h"

#include "Renderer/Entities/RectVao.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Util/ImGuiHelper.h"

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
		if (character->CanMoveLeft()) {
			character->AddPosition(Vec2(-1,  0) * Time::GetDelta() * 120.0f);
		}
	}

	if (Input::KeyDown(Key::D)) {
		if (character->CanMoveRight()) {
			character->AddPosition(Vec2( 1,  0) * Time::GetDelta() * 120.0f);
		} 
	}

	if (Input::KeyDown(Key::Space)) {
		if (character->OnGround()) {
			character->Jump();
		}
	}

	camera->SetPosition(character->GetPosition());
}

void Engine::Render() {
	if (Input::MouseButtonDown(Button::Left)) {
		BlockSettingData& settingBlock = world->GetMap()->SetBlock(camera->GetViewMatrix(), BlockType::Empty);
		if (settingBlock.IsSet()) {
			worldRenderer->mapPipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			worldRenderer->mapPipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);
			worldRenderer->ForceRerender();
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		BlockSettingData& settingBlock = world->GetMap()->SetBlock(camera->GetViewMatrix(), BlockType::Dirt);
		if (settingBlock.IsSet()) {
			worldRenderer->mapPipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			worldRenderer->mapPipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);
			worldRenderer->ForceRerender();
		}
	}

	{
		FORGIO_PROFILER_NAMED_SCOPE("Current \"Physics\"");

		character->Update(Time::GetDelta());
		character->CheckCollisions(world->GetMap(), camera);		
		// character->CheckCollisions(world->GetMap(), camera, debugRenderer);
		// debugRenderer->AddQuad(character->GetPosition() + Vec2(8.0f, 0.0f), character->GetPosition() + Vec2(3 * 16.0f, 4 * 16.0f) - Vec2(8.0f, 0.0f));

		const Pos blockPos = world->GetMap()->WindowCoordsToBlockCoords(
			camera->GetPositionOnScreen(character->GetPosition()), 
			Window::GetSpace(), 
			camera->GetViewMatrix()
		);

		for (int x = -1; x < 1; x++) {
			for (int y = -1; y < 2; y++) {
				const bool emptyBlock = world->GetMap()->BlockIs(blockPos.x + x, blockPos.y + y, BlockType::Empty);
				const bool emptyWall = world->GetMap()->WallIs(blockPos.x + x, blockPos.y + y, WallType::Empty);
				if (emptyBlock && emptyWall) {
					worldRenderer->mapPipeline.color->GetAdditionalLightData().push_back(character->GetPosition() + Vec2(0, 64));
				}
			}
		}
	}

	worldRenderer->Render([&]() {
		characterRenderer->Render({ character }, camera);
	});

	debugRenderer->Render(camera->GetViewMatrix());

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(worldRenderer->mapPipeline.color->info.chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Lights: " + std::to_string(worldRenderer->mapPipeline.color->GetLightData().size())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		auto visibleChunks = world->GetMap()->GetVisibleChunks();
		ImGui::Text(("Position: " + std::to_string(camera->GetPosition().x) + ' ' + std::to_string(camera->GetPosition().y)).c_str());
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