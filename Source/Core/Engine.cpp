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

	ImageAsset image("Assets/Images/Map2.png");
	tileMap = std::make_shared<Texture>(
		image.GetSize(),
		image.GetData(),
		GL_RGBA,
		image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
		GL_UNSIGNED_BYTE,
		std::vector<Texture::param_t> {
			{ ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
			{ ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
		}
	);

	tileVao = std::make_shared<Vao>(
		Primitives::Quad::vertices,
		Vertex::GetLayout(),
		Primitives::Quad::indices
	);

	TextAsset vsCode("Assets/Shaders/Terrain/Default.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/Default.fs");

	viewPos = map->GetCenter() * 16.0f;
	viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

	shader = std::make_shared<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_Tile");
	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
		shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
		shader->SetVec2("u_Tile", Math::ToPtr(Vec2(1, 1)));
	shader->Unbind();
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
	if (Window::KeyPressed(GLFW_KEY_W)) viewPos += Vec2( 0,  1) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_S)) viewPos += Vec2( 0, -1) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_A)) viewPos += Vec2(-1,  0) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_D)) viewPos += Vec2( 1,  0) * 4.0f;

	viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));
}

void Engine::Render() {
	std::vector<Map::chunk_t> chunks = { 
		map->WhatBlocks({ 20, 20 }),
		map->WhatBlocks({ 21, 21 }) 
	};

	shader->Bind();
		tileVao->Bind();
			tileMap->Bind();
				for (int i = 0; i < chunks.size(); i++) {
					for (int x = chunks[i].x.start; x < chunks[i].x.end; x++) {
						for (int y = chunks[i].y.start; y < chunks[i].y.end; y++) {
							BlockType type = map->blocks[x][y];
							Vec2 textureOffset = Vec2(1, 1);
							Vec2 pos = { x * 16, y * 16 };
							shader->SetVec2("u_Tile", Math::ToPtr(textureOffset));
							shader->SetVec2("u_Pos", Math::ToPtr(pos));
							glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
						}
					}
				}
			tileMap->Unbind();
		tileVao->Unbind();
	shader->Unbind();
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