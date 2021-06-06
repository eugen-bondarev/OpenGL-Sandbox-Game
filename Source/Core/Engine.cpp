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

	map = CreateRef<Map>(Size(40, 40), Size(25, 25), 16.0f);
	view.position = map->GetCenter() * map->GetBlockSize();
	view.matrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(view.position, 0.0f)));
	map->CalculateVisibleChunks(view.position);

	visibleBlocks = map->GetVisibleChunks();
	lastVisibleBlocks = map->GetVisibleChunks();

	pipeline.color = CreateRef<ColorPass>(map);
	pipeline.light = CreateRef<LightPass>(pipeline.color->GetMapRenderer());
	pipeline.composition = CreateRef<CompositionPass>();
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
	if (Input::KeyDown(Key::W)) view.position += Vec2( 0,  1) * Time::GetDelta() * 300.0f;
	if (Input::KeyDown(Key::S)) view.position += Vec2( 0, -1) * Time::GetDelta() * 300.0f;
	if (Input::KeyDown(Key::A)) view.position += Vec2(-1,  0) * Time::GetDelta() * 300.0f;
	if (Input::KeyDown(Key::D)) view.position += Vec2( 1,  0) * Time::GetDelta() * 300.0f;

	view.matrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(view.position, 0.0f)));
}

void Engine::Render() {
	if (Input::MouseButtonDown(Button::Left)) {
		auto& settingBlock = map->SetBlock(view.matrix, BlockType::Empty);
		if (settingBlock.IsSet()) {
			pipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			pipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);

			rerender = true;
			chunksChanged = true;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		auto& settingBlock = map->SetBlock(view.matrix, BlockType::Dirt);
		if (settingBlock.IsSet()) {
			pipeline.color->GetMapRenderer()->RerenderChunk(settingBlock.chunk);
			pipeline.color->GetMapRenderer()->UpdateNeighborChunks(settingBlock.chunk, settingBlock.block);

			rerender = true;
			chunksChanged = true;
		}
	}

	if (view.lastPosition != view.position) {
		rerender = true;
		map->CalculateVisibleChunks(view.position);
		visibleBlocks = map->GetVisibleChunks();
		view.lastPosition = view.position;
	}

	if (lastVisibleBlocks != visibleBlocks) {
		lastVisibleBlocks = visibleBlocks;
		chunksChanged = true;
	}

	if (rerender) {
		pipeline.color->Execute(view.matrix, view.position);
		pipeline.light->Execute(view.matrix, view.position, pipeline.color->GetLightData());
		rerender = false;
	}

	pipeline.composition->Execute(pipeline.color, pipeline.light);

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(pipeline.color->info.chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Lights: " + std::to_string(pipeline.color->GetLightData().size())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		auto visibleChunks = map->GetVisibleChunks();
		ImGui::Text(("Position: " + std::to_string(view.position.x) + ' ' + std::to_string(view.position.y)).c_str());
		ImGui::Text(("Chunk x: " + std::to_string(visibleChunks.x.start) + ' ' + std::to_string(visibleChunks.x.end)).c_str());
		ImGui::Text(("Chunk y: " + std::to_string(visibleChunks.y.start) + ' ' + std::to_string(visibleChunks.y.end)).c_str());
	ImGui::End();

	chunksChanged = false;
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