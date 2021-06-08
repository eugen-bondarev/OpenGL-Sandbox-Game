#include "Engine.h"

#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"
#include "Core/Input/Input.h"

#include "Renderer/Entities/RectVao.h"

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
	if (Input::KeyDown(Key::W)) camera->AddPosition(Vec2( 0,  1) * Time::GetDelta() * 300.0f);
	if (Input::KeyDown(Key::S)) camera->AddPosition(Vec2( 0, -1) * Time::GetDelta() * 300.0f);
	if (Input::KeyDown(Key::A)) camera->AddPosition(Vec2(-1,  0) * Time::GetDelta() * 300.0f);
	if (Input::KeyDown(Key::D)) camera->AddPosition(Vec2( 1,  0) * Time::GetDelta() * 300.0f);
}

void Engine::Render() {
	if (Input::MouseButtonDown(Button::Left)) {
		auto& settingBlock = world->GetMap()->SetBlock(camera->GetViewMatrix(), BlockType::Empty);
		if (settingBlock.IsSet()) {
			worldRenderer->mapPipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			worldRenderer->mapPipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);
			worldRenderer->ForceRerender();
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

	worldRenderer->Render();

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