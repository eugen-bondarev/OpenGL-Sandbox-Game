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

	map = std::make_shared<Map>(Size{12, 12}, Size{42, 42}); // ~ 500 x 500

	// ImageAsset image("Assets/Images/Map2.png");
	// tileMap = std::make_shared<Texture>(
	// 	image.GetSize(),
	// 	image.GetData(),
	// 	GL_RGBA,
	// 	image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
	// 	GL_UNSIGNED_BYTE,
	// 	std::vector<Texture::param_t> {
	// 		{ ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
	// 		{ ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	// 	}
	// );

	// tileVao = std::make_shared<Vao>(
	// 	Primitives::Quad::vertices,
	// 	Vertex::GetLayout(),
	// 	Primitives::Quad::indices
	// );

	// TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	// TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");

	// // viewPos = map->GetCenter() * 16.0f;
	// viewPos = (Vec2(1, 0) * 12.0f) * 16.0f;
	// viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));
	// // viewMatrix = Mat4(1);

	// float chunkSize = 192.0f;
	// float halfChunkSize = chunkSize / 2.0f;
	// projMatrix = Math::Ortho(-halfChunkSize, halfChunkSize, -halfChunkSize, halfChunkSize);

	// shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_Tile");
	// shader->Bind();
	// 	shader->SetMat4x4("u_Proj", Math::ToPtr(projMatrix));
	// 	shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
	// 	shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	// shader->Unbind();

	// std::vector<chunk_t> chunks = { 
	// 	map->WhatBlocks({ 20, 20 }),
	// 	map->WhatBlocks({ 21, 21 }) 
	// };
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
	if (Window::KeyPressed(GLFW_KEY_W)) map->viewPos += Vec2( 0,  1) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_S)) map->viewPos += Vec2( 0, -1) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_A)) map->viewPos += Vec2(-1,  0) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_D)) map->viewPos += Vec2( 1,  0) * 4.0f;

	map->viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(map->viewPos, 0.0f)));
}

void Engine::Render() {
	ImGui::Begin("Info");
		ImGui::Image((void*)(intptr_t)map->chunks[2][0].GetTexture()->GetHandle(), ImVec2(192, 192), ImVec2(0, 0), ImVec2(1, -1));
	ImGui::End();

	Pos chunkToRender { 2, 0 };
	
	std::vector<chunk_t> chunks = { 
		map->WhatBlocks(chunkToRender),
	};
	glViewport(0.0f, 0.0f, 192, 192);

	map->chunks[2][0].Prepare();
	map->chunks[2][0].Rerender();

	map->chunks[3][0].Prepare();
	map->chunks[3][0].Rerender();

	// map->chunks[0][0].Rerender();

	// map->viewPos = (chunkToRender * map->GetChunkSize()) * BLOCK_SIZE;
	// map->viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(map->viewPos, 0.0f)));

	// map->chunkFbo->Bind();
	// map->chunkFbo->Clear();
	// map->shader->Bind();
	// map->shader->SetMat4x4("u_View", Math::ToPtr(map->chunks[2][0].viewMatrix));
	// 	map->tileVao->Bind();
	// 		map->tileMap->Bind();
	// 			for (int i = 0; i < chunks.size(); i++) {
	// 				for (int x = chunks[i].x.start; x < chunks[i].x.end; x++) {
	// 					for (int y = chunks[i].y.start; y < chunks[i].y.end; y++) {
	// 						BlockType type = map->blocks[x][y];
	// 						Vec2 textureOffset = Vec2(0, 0);
	// 						Vec2 pos = Vec2(x * BLOCK_SIZE, y * BLOCK_SIZE);
	// 						Vec2 chunkCenter = map->GetChunkSize() / 2.0f * BLOCK_SIZE - BLOCK_SIZE / 2.0f;
	// 						Vec2 shiftedPosition = pos - chunkCenter;
	// 						map->shader->SetVec2("u_Tile", Math::ToPtr(textureOffset));
	// 						map->shader->SetVec2("u_Pos", Math::ToPtr(shiftedPosition));
	// 						glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	// 					}
	// 				}
	// 			}
	// 		map->tileMap->Unbind();
	// 	map->tileVao->Unbind();
	// map->shader->Unbind();
	// map->chunkFbo->Unbind();
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