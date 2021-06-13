#include "Engine.h"

#include "Core/Window.h"
#include "Core/Time.h"
#include "Core/Gui.h"
#include "Core/Input/Input.h"

#include "Renderer/Entities/RectVao.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Util/ImGuiHelper.h"

#include "Werwel/GraphicsContext.h"

Engine::Engine() {
	Window::Create();
	Gui::Create();
	Input::Create(Window::GetGlfwWindow());
	Primitives::Rect::Create();

	debugRenderer = CreateRef<DebugRenderer>();
}

inline static std::vector<Vertex2D> Verts(float tileSizeX, float tileSizeY, bool flipUVs = false) {
	return {
		{ {  tileSizeX / 2,  tileSizeY / 2 }, { 1, 1 } },
		{ { -tileSizeX / 2,  tileSizeY / 2 }, { 0, 1 } },
		{ {  tileSizeX / 2, -tileSizeY / 2 }, { 1, 0 } },
		{ { -tileSizeX / 2, -tileSizeY / 2 }, { 0, 0 } },
	};
}

inline static std::vector<int> Inds = { 0, 1, 2, 2, 1, 3 };

void Engine::InitResources() {
	FORGIO_PROFILER_SCOPE();

	map = CreateRef<Map>(Size(16, 16), Size(25, 25));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());

	map->CalculateVisibleChunks(camera->GetPosition());

	TextAsset vsCode("Assets/MapTest.vs");
	TextAsset fsCode("Assets/MapTest.fs");
	shader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_Proj", "u_View", "u_Position", "blockData"
	);

	shader->Bind();
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));

	const ImageAsset tileMapTexture("Assets/Images/Map.png");
	tileMap = CreateRef<Werwel::Texture>(
		Werwel::Size(tileMapTexture.GetSize().x, tileMapTexture.GetSize().y),
		tileMapTexture.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);
	
	const auto& vers = Primitives::Block::Vertices(16, 16);
	const auto& inds = Primitives::Block::indices;

	vao = CreateRef<Werwel::VAO>();
	vao->Bind();
		vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
		vbo = vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, 12288, sizeof(Vec4), nullptr, std::vector<Werwel::VertexBufferLayout> { { 4, sizeof(Vec4), 0, 1 } });

	PopulateBlockData(true);

	visibleChunks = map->GetVisibleChunks();
}

void Engine::PopulateBlockData(bool firstTime) {
	FORGIO_PROFILER_NAMED_SCOPE("Building blocks!");

	const auto& chunks = map->GetVisibleChunks();
	const auto& blocks = map->GetBlocks();

	for (int x = chunks.x.start; x < chunks.x.end; x++) {
		for (int y = chunks.y.start; y < chunks.y.end; y++) {			

			map->chunks[x][y].shown = true;

			int firstBlockX = x * 16.0f;
			int lastBlockX = (x + 1) * 16.0f;
			int firstBlockY = y * 16.0f;
			int lastBlockY = (y + 1) * 16.0f;

			int pos = blocksData.size();
			map->chunks[x][y].memPos = pos;

			for (int i = firstBlockX; i < lastBlockX; i++) {
				for (int j = firstBlockY; j < lastBlockY; j++) {
					// if (blocks[i][j] != BlockType::Empty) {
						blocksData.emplace_back(i * 16.0f, j * 16.0f, 1, 0);
					// } else {
					// 	blocksData.emplace_back(0, 0, 0, 0);
					// 	size += 1;
					// }
				}
			}
			// we have position and size..
		}
	}

	vbo->Update(blocksData, blocksData.size());
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::BeginFrame();
	Gui::BeginFrame();
	Time::BeginFrame();
	Input::BeginFrame();

	// debugRenderer->ClearLines();
}

void Engine::Control() {
	if (Input::KeyDown(Key::W)) {
		camera->SetPosition(camera->GetPosition() + Vec2(0, 1) * Time::GetDelta() * 300.0f);
	}

	if (Input::KeyDown(Key::A)) {
		camera->SetPosition(camera->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * 300.0f);
	}

	if (Input::KeyDown(Key::S)) {
		camera->SetPosition(camera->GetPosition() + Vec2(0, -1) * Time::GetDelta() * 300.0f);
	}

	if (Input::KeyDown(Key::D)) {
		camera->SetPosition(camera->GetPosition() + Vec2(1, 0) * Time::GetDelta() * 300.0f);
	}
}

void Engine::OnVisibleChunksChange() {
	if (lastVisibleChunks.x.start < visibleChunks.x.start); //leftGone = true;
	if (lastVisibleChunks.x.start > visibleChunks.x.start); //leftNew = true;
	if (lastVisibleChunks.x.end < visibleChunks.x.end); //rightNew = true;
	if (lastVisibleChunks.x.end > visibleChunks.x.end); //rightGone = true;
	if (lastVisibleChunks.y.start < visibleChunks.y.start); //botGone = true;

	const auto& blocks = map->GetBlocks();

	if (lastVisibleChunks.y.start < visibleChunks.y.start) {
		for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
			int oldChunkIndex = lastVisibleChunks.y.start;
			int newChunkIndex = visibleChunks.y.end - 1;

			auto& oldChunk = map->chunks[x][oldChunkIndex];
			auto& newChunk = map->chunks[x][newChunkIndex];

			oldChunk.shown = false;
			newChunk.shown = true;
 
			std::vector<Vec4> newBlocks;
			bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
			bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

			for (int i = newChunkBounds.x.start; i < newChunkBounds.x.end; i++) {
				for (int j = newChunkBounds.y.start; j < newChunkBounds.y.end; j++) {
					newBlocks.emplace_back(i * 16.0f, j * 16.0f, 1, 0);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo->GetHandle());
			glBufferSubData(GL_ARRAY_BUFFER, oldChunk.memPos * sizeof(Vec4), 16 * 16 * sizeof(Vec4), newBlocks.data());
			newChunk.memPos = oldChunk.memPos;
		}
	} else if (lastVisibleChunks.y.start > visibleChunks.y.start) {
		for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
			int oldChunkIndex = lastVisibleChunks.y.end - 1;
			int newChunkIndex = visibleChunks.y.start;

			auto& oldChunk = map->chunks[x][oldChunkIndex];
			auto& newChunk = map->chunks[x][newChunkIndex];

			oldChunk.shown = false;
			newChunk.shown = true;
 
			std::vector<Vec4> newBlocks;
			bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
			bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

			for (int i = newChunkBounds.x.start; i < newChunkBounds.x.end; i++) {
				for (int j = newChunkBounds.y.start; j < newChunkBounds.y.end; j++) {
					newBlocks.emplace_back(i * 16.0f, j * 16.0f, 1, 0);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo->GetHandle());
			glBufferSubData(GL_ARRAY_BUFFER, oldChunk.memPos * sizeof(Vec4), 16 * 16 * sizeof(Vec4), newBlocks.data());
			newChunk.memPos = oldChunk.memPos;
		}
	}

	if (lastVisibleChunks.x.end < visibleChunks.x.end) {
		for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
			int oldChunkIndex = lastVisibleChunks.x.start;
			int newChunkIndex = visibleChunks.x.end - 1;

			auto& oldChunk = map->chunks[oldChunkIndex][y];
			auto& newChunk = map->chunks[newChunkIndex][y];

			oldChunk.shown = false;
			newChunk.shown = true;
 
			std::vector<Vec4> newBlocks;
			bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
			bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

			for (int i = newChunkBounds.x.start; i < newChunkBounds.x.end; i++) {
				for (int j = newChunkBounds.y.start; j < newChunkBounds.y.end; j++) {
					newBlocks.emplace_back(i * 16.0f, j * 16.0f, 1, 0);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo->GetHandle());
			glBufferSubData(GL_ARRAY_BUFFER, oldChunk.memPos * sizeof(Vec4), 16 * 16 * sizeof(Vec4), newBlocks.data());
			newChunk.memPos = oldChunk.memPos;
		}
	} else if (lastVisibleChunks.x.end > visibleChunks.x.end) {
		for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
			int oldChunkIndex = lastVisibleChunks.x.end - 1;
			int newChunkIndex = visibleChunks.x.start;

			auto& oldChunk = map->chunks[oldChunkIndex][y];
			auto& newChunk = map->chunks[newChunkIndex][y];

			oldChunk.shown = false;
			newChunk.shown = true;
 
			std::vector<Vec4> newBlocks;
			bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
			bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

			for (int i = newChunkBounds.x.start; i < newChunkBounds.x.end; i++) {
				for (int j = newChunkBounds.y.start; j < newChunkBounds.y.end; j++) {
					newBlocks.emplace_back(i * 16.0f, j * 16.0f, 1, 0);
				}
			}

			glBindBuffer(GL_ARRAY_BUFFER, vbo->GetHandle());
			glBufferSubData(GL_ARRAY_BUFFER, oldChunk.memPos * sizeof(Vec4), 16 * 16 * sizeof(Vec4), newBlocks.data());
			newChunk.memPos = oldChunk.memPos;
		}
	}
}

void Engine::Render() {
	Werwel::GraphicsContext::Clear();

	camera->OnPositionChange([&]() {
		map->CalculateVisibleChunks(camera->GetPosition());
		visibleChunks = map->GetVisibleChunks();

		if (lastVisibleChunks != visibleChunks) {
			OnVisibleChunksChange();
			lastVisibleChunks = visibleChunks;
		}

		// blocksData.clear();		
		// PopulateBlockData(false);

		// {
		// 	FORGIO_PROFILER_NAMED_SCOPE("Updating buffer");
		// 	vbo->Update(blocksData, blocksData.size());
		// 	glFinish();
		// }
	});

	{
		FORGIO_PROFILER_NAMED_SCOPE("Binding");
		shader->Bind();
		shader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));
		tileMap->Bind();
		vao->Bind();
		vao->GetIndexBuffer()->Bind();
		glFinish();
	}

	{
		FORGIO_PROFILER_NAMED_SCOPE("Rendering!");
		glDrawElementsInstanced(GL_TRIANGLES, vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, blocksData.size());
		glFinish();
	}
	
	debugRenderer->Render(camera->GetViewMatrix());

	ImGui::Begin("Info");
		ImGui::Text(("FPS:" + std::to_string(Time::GetFps())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		ImGui::Text(("x: " + std::to_string(visibleChunks.x.start) + " " + std::to_string(visibleChunks.x.end)).c_str());
		ImGui::Text(("y: " + std::to_string(visibleChunks.y.start) + " " + std::to_string(visibleChunks.y.end)).c_str());

		ImGui::Text(("Left new: "  + std::string(leftNew ? "true" : "false")).c_str());
		ImGui::Text(("Left gone: " + std::string(leftGone ? "true" : "false")).c_str());
		ImGui::Text(("Right new: " + std::string(rightNew ? "true" : "false")).c_str());
		ImGui::Text(("Right gone: " + std::string(rightGone ? "true" : "false")).c_str());
		ImGui::Text(("Bot gone: " + std::string(botGone ? "true" : "false")).c_str());
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