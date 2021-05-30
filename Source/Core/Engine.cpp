#include "Engine.h"

#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"
#include "Core/Input/Input.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"

#include "Renderer/Entities/RectVao.h"

#include "Gpu/GraphicsContext.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
}

void Engine::InitResources() {
	Primitives::Rect::Create();

	map = std::make_shared<Map>(Size(16, 16), Size(42, 42));
	pipeline.color = std::make_shared<ColorPass>(map);
	pipeline.light = std::make_shared<LightPass>(pipeline.color->GetMapRenderer());
	pipeline.composition = std::make_shared<CompositionPass>();

	view.position = map->GetCenter() * BLOCK_SIZE;
	view.matrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(view.position, 0.0f)));
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
	if (Input::KeyDown(KEY_W)) view.position += Vec2( 0,  1) * Time::GetDelta() * 300.0f;
	if (Input::KeyDown(KEY_S)) view.position += Vec2( 0, -1) * Time::GetDelta() * 300.0f;
	if (Input::KeyDown(KEY_A)) view.position += Vec2(-1,  0) * Time::GetDelta() * 300.0f;
	if (Input::KeyDown(KEY_D)) view.position += Vec2( 1,  0) * Time::GetDelta() * 300.0f;

	view.matrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(view.position, 0.0f)));
}

void Engine::Render() {
	if (Input::MouseButtonDown(MOUSE_BUTTON_LEFT)) {
		const Vec2 block = map->WindowCoordsToBlockCoords(Window::GetMousePosition(), Window::GetSpace(), view.matrix);

		if (map->blocks[block.x][block.y] != BlockType::Empty) {
			map->blocks[block.x][block.y] = BlockType::Empty;

			const Pos chunk = map->WhatChunk(block);
			pipeline.color->GetMapRenderer()->chunks[chunk.x][chunk.y].Rerender();
			rerender = true;
		}
	}

	if (Input::MouseButtonDown(MOUSE_BUTTON_RIGHT)) {
		const Vec2 block = map->WindowCoordsToBlockCoords(Window::GetMousePosition(), Window::GetSpace(), view.matrix);

		if (map->blocks[block.x][block.y] == BlockType::Empty) {
			map->blocks[block.x][block.y] = BlockType::Dirt;

			const Pos chunk = map->WhatChunk(block);
			pipeline.color->GetMapRenderer()->chunks[chunk.x][chunk.y].Rerender();
			rerender = true;
		}
	}

	if (view.lastPosition != view.position) {
		rerender = true;
		view.lastPosition = view.position;
	}

	if (rerender) {
		pipeline.color->Execute(view.matrix, view.position);
		pipeline.light->Execute(pipeline.color, map, view.matrix, view.position);
		rerender = false;
	}

	pipeline.composition->Execute(pipeline.color, pipeline.light);

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(pipeline.color->info.chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Lights: " + std::to_string(pipeline.color->light.size())).c_str());
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