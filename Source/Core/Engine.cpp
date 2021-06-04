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
	map = CreateRef<Map>(Size(16, 16), Size(25, 25), 16.0f);
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

void Update(MapRenderer::chunks_t& chunks, const Chunk& chunk, const Vec2& block) {
	const auto& bounds = chunk.GetBoudns();

	bool left { false };
	bool right { false };
	bool up { false };
	bool down { false };

	if (bounds.x.start + 1 >= block.x) {
		left = true;
	} else if (bounds.x.end - 1 <= block.x) {
		right = true;
	}

	if (bounds.y.start + 1 >= block.y) {
		up = true;
	} else if (bounds.y.end - 1 <= block.y) {
		down = true;
	}

	if (left) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x - 1][chunk.GetChunkPos().y];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	} else if (right) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x + 1][chunk.GetChunkPos().y];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	}

	if (up) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x][chunk.GetChunkPos().y - 1];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	} else if (down) {
		auto& chunkToUpdate = chunks[chunk.GetChunkPos().x][chunk.GetChunkPos().y + 1];
		chunkToUpdate.Rerender();
		chunkToUpdate.highlight = true;
	}
}

void Engine::Render() {
	if (Input::MouseButtonDown(Button::Left)) {
		const Vec2 block = map->WindowCoordsToBlockCoords(Window::GetMousePosition(), Window::GetSpace(), view.matrix);

		if (map->blocks[block.x][block.y] != BlockType::Empty) {
			map->blocks[block.x][block.y] = BlockType::Empty;

			const Pos chunkCoordinates = map->WhatChunk(block);
			auto& chunk = pipeline.color->GetMapRenderer()->chunks[chunkCoordinates.x][chunkCoordinates.y];
			chunk.Rerender();
			chunk.highlight = true;

			Update(pipeline.color->GetMapRenderer()->chunks, chunk, block);

			rerender = true;
			chunksChanged = true;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		const Vec2 block = map->WindowCoordsToBlockCoords(Window::GetMousePosition(), Window::GetSpace(), view.matrix);

		if (map->blocks[block.x][block.y] == BlockType::Empty) {
			map->blocks[block.x][block.y] = BlockType::Dirt;

			const Pos chunkCoordinates = map->WhatChunk(block);
			auto& chunk = pipeline.color->GetMapRenderer()->chunks[chunkCoordinates.x][chunkCoordinates.y];
			chunk.Rerender();
			chunk.highlight = true;

			Update(pipeline.color->GetMapRenderer()->chunks, chunk, block);

			rerender = true;
			chunksChanged = true;
		}
	}

	if (Input::KeyPressed(Key::Space)) {
		const auto& bounds = map->GetVisibleChunks();
		for (int x = bounds.x.start; x < bounds.x.end; x++) {
			for (int y = bounds.y.start; y < bounds.y.end; y++) {
				auto& chunk = pipeline.color->GetMapRenderer()->chunks[x][y];
				chunk.highlight = false;
			}
		}
		rerender = true;
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
		pipeline.light->Execute(view.matrix, view.position, pipeline.color->light);
		rerender = false;
	}

	pipeline.composition->Execute(pipeline.color, pipeline.light);

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(pipeline.color->info.chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(Time::GetFps())).c_str());
		ImGui::Text(("Lights: " + std::to_string(pipeline.color->light.size())).c_str());
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