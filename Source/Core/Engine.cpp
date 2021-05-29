#include "Engine.h"

#include "Core/Window.h"
#include "Core/Gui.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"

#include "Renderer/Entities/RectVao.h"

#include "Gpu/GraphicsContext.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
}

void Engine::InitResources() {
	Primitives::Rect::Create();

	map = std::make_shared<Map>(Size{16, 16}, Size{42, 42}); // ~ 500 x 500

	for (int x = 0; x < map->GetAmountOfChunks().x; x++) {
		for (int y = 0; y < map->GetAmountOfChunks().y; y++) {
			const float currentTime = static_cast<float>(glfwGetTime());
			map->chunks[x][y].Rerender();
			const float elapsedTime = static_cast<float>(glfwGetTime()) - currentTime;
		}
	}

	const TextAsset chunkShaderVsCode("Assets/Shaders/Terrain/Chunk.vs");
	const TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Chunk.fs");
	chunkShader = std::make_shared<Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos");
	chunkVao = std::make_shared<Vao>(Primitives::Pixel::vertices, Vertex::GetLayout(), Primitives::Pixel::indices);

	viewPos = map->GetCenter() * BLOCK_SIZE;
	viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

	const Mat4 chunkModelMatrix = Math::Scale(Mat4(1), Vec3(map->GetChunkSizePixels().x, -map->GetChunkSizePixels().y, 1.0f));

	chunkShader->Bind();
		chunkShader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
		chunkShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	chunkShader->Unbind();
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::Clear();
	Window::PollEvents();
	Gui::Begin();

	currentTime = static_cast<float>(glfwGetTime());
	delta = currentTime - lastTime;
	lastTime = currentTime;
}

void Engine::Control() {
	if (Window::KeyPressed(GLFW_KEY_W)) viewPos += Vec2( 0,  1) * delta * 300.0f;
	if (Window::KeyPressed(GLFW_KEY_S)) viewPos += Vec2( 0, -1) * delta * 300.0f;
	if (Window::KeyPressed(GLFW_KEY_A)) viewPos += Vec2(-1,  0) * delta * 300.0f;
	if (Window::KeyPressed(GLFW_KEY_D)) viewPos += Vec2( 1,  0) * delta * 300.0f;

	viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));
}

bounds_t Engine::GetVisibleChunks(std::shared_ptr<Map>& map, Pos viewPos) {
	const Pos middle = map->WhatChunk(map->GetCenter());
	const Vec2 centeredViewPos = viewPos - map->GetCenter() * BLOCK_SIZE;
	const Vec2 additionalBlocks = Vec2(2, 2);
	const Vec2 chunkSizeInPixels = map->GetChunkSize() * BLOCK_SIZE;
	const Vec2 shift = (Window::GetSize() / chunkSizeInPixels / 2.0f);
	
	bounds_t bounds;
	bounds.x.start = middle.x - shift.x + centeredViewPos.x / chunkSizeInPixels.x - additionalBlocks.x;
	bounds.x.end   = middle.x + shift.x + centeredViewPos.x / chunkSizeInPixels.x + additionalBlocks.x;
	bounds.y.start = middle.y - shift.y + centeredViewPos.y / chunkSizeInPixels.y - additionalBlocks.y;
	bounds.y.end   = middle.y + shift.y + centeredViewPos.y / chunkSizeInPixels.y + additionalBlocks.y;

	return bounds;
}

void Engine::Render() {
	if (glfwGetMouseButton(Window::GetGlfwWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
		const Pos mousePos = Window::GetMousePosition();
		const Vec2 block = map->WindowCoordsToBlockCoords(mousePos, Window::GetSpace(), viewMatrix);

		if (map->blocks[block.x][block.y] != BlockType::Empty) {
			map->blocks[block.x][block.y] = BlockType::Empty;

			const Pos chunk = map->WhatChunk(block);
			map->chunks[chunk.x][chunk.y].Rerender();
		}
	}

	if (glfwGetMouseButton(Window::GetGlfwWindow(), GLFW_MOUSE_BUTTON_RIGHT)) {
		const Pos mousePos = Window::GetMousePosition();
		const Vec2 block = map->WindowCoordsToBlockCoords(mousePos, Window::GetSpace(), viewMatrix);

		if (map->blocks[block.x][block.y] == BlockType::Empty) {
			map->blocks[block.x][block.y] = BlockType::Dirt;

			const Pos chunk = map->WhatChunk(block);
			map->chunks[chunk.x][chunk.y].Rerender();
		}
	}

	chunksRendered = 0;

	GraphicsContext::ClearColor({ 224 / 255.0f, 236 / 255.0f, 255 / 255.0f, 1.0f });

	chunkShader->Bind();
	chunkShader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
		chunkVao->Bind();
			bounds_t bounds = GetVisibleChunks(map, viewPos);
			
			for (int x = bounds.x.start; x < bounds.x.end; x++) {
				for (int y = bounds.y.start; y < bounds.y.end; y++) {
					map->chunks[x][y].Render(chunkShader);					
					chunksRendered += 1;
				}
			}
			
		chunkVao->Unbind();
	chunkShader->Unbind();

	ImGui::Begin("Info");
		ImGui::Text(("Chunks rendered: " + std::to_string(chunksRendered)).c_str());
		ImGui::Text(("Fps: " + std::to_string(fps)).c_str());
	ImGui::End();
}

void Engine::EndFrame() {
	fpsTimer += delta;
	if (fpsTimer >= 1.0f) {
		fps = 1.0f / delta;
		fpsTimer = 0;
	}

	Gui::End();
	Window::SwapBuffers();
}

Engine::~Engine() {
	Gui::Destroy();
	Window::Destroy();
}