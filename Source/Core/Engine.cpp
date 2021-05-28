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

	map = std::make_shared<Map>(Size{16, 16}, Size{21, 21}); // ~ 500 x 500

	for (int x = 0; x < map->GetAmountOfChunks().x; x++) {
		for (int y = 0; y < map->GetAmountOfChunks().y; y++) {
			const float currentTime = static_cast<float>(glfwGetTime());
			map->chunks[x][y].Rerender();
			const float elapsedTime = static_cast<float>(glfwGetTime()) - currentTime;
		}
	}

	TextAsset chunkShaderVsCode("Assets/Shaders/Terrain/Chunk.vs");
	TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Chunk.fs");
	chunkShader = std::make_shared<Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos");
	squareVao = std::make_shared<Vao>(Primitives::Pixel::vertices, Vertex::GetLayout(), Primitives::Pixel::indices);

	viewPos = map->GetCenter() * 16.0f;
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

void Engine::Render() {
	if (glfwGetMouseButton(Window::GetGlfwWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
		const Pos mousePos = Window::GetMousePosition();
		const Vec2 block = map->WindowCoordsToBlockCoords(mousePos, Window::GetSpace(), viewMatrix);
		map->blocks[block.x][block.y] = BlockType::Empty;

		const Pos chunk = map->WhatChunk(block);
		map->chunks[chunk.x][chunk.y].Rerender();
	}

	chunksRendered = 0;

	chunkShader->Bind();
	chunkShader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
		squareVao->Bind();
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
			
			for (int x = bounds.x.start; x < bounds.x.end; x++) {
				for (int y = bounds.y.start; y < bounds.y.end; y++) {
					map->chunks[x][y].GetTexture()->Bind();
						const Vec2 pos = Vec2(x, y) * map->GetChunkSizePixels();
						Mat4 model = Math::Translate(Mat4(1), Vec3(pos, 1.0f));
						model = Math::Scale(model, Vec3(map->GetChunkSizePixels().x, -map->GetChunkSizePixels().y, 1.0f));
						chunkShader->SetMat4x4("u_Model", Math::ToPtr(model));
						glDrawElements(GL_TRIANGLES, squareVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
					map->chunks[x][y].GetTexture()->Unbind();

					chunksRendered += 1;
				}
			}
			
		squareVao->Unbind();
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