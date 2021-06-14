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

	// debugRenderer = CreateRef<DebugRenderer>();
	Linow::Init(Math::ToPtr(Window::GetSpace()));
}

static bool TopBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return y < blocks[x].size() - 1 && blocks[x][y + 1] == BlockType::Empty;
}

static bool LeftBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return x > 0 && blocks[x - 1][y] == BlockType::Empty;
}

static bool BottomBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return y > 0 && blocks[x][y - 1] == BlockType::Empty;
}

static bool RightBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return x < blocks.size() - 1 && blocks[x + 1][y] == BlockType::Empty;
}

static Vec2 PickRightAngularTile(const blocks_t& blocks, int x, int y) {
  #define TOP()     TopBlockIsEmpty(blocks, x, y)
  #define LEFT()    LeftBlockIsEmpty(blocks, x, y)
  #define BOTTOM()  BottomBlockIsEmpty(blocks, x, y)
  #define RIGHT()   RightBlockIsEmpty(blocks, x, y)

  if (TOP() && LEFT() && BOTTOM() && RIGHT())
    return { 1, 2 };

  if (LEFT() && TOP() && RIGHT())
    return { -1, 3 };

  if (LEFT() && BOTTOM() && RIGHT())
    return { -1, 4 };

  if (TOP() && RIGHT() && BOTTOM())
    return { 0, 3 };

  if (TOP() && LEFT() && BOTTOM())
    return { 0, 4 };

  if (TOP() && LEFT())
    return { -1, -1 };

  if (TOP() && RIGHT())
    return { 1, -1 };

  if (BOTTOM() && LEFT())
    return { -1, 1 };

  if (BOTTOM() && RIGHT())
    return { 1, 1 };

  if (TOP() && BOTTOM())
    return { 0, 2 };

  if (LEFT() && RIGHT())
    return { -1, 2 };

  if (TOP())
    return { 0, -1 };

  if (BOTTOM())
    return { 0, 1 };

  if (LEFT())
    return { -1, 0 };

  if (RIGHT())
    return { 1, 0 };

  #undef TOP
  #undef BOTTOM
  #undef LEFT
  #undef RIGHT

  return Vec2(0, 0);
}

void Engine::InitResources() {
	FORGIO_PROFILER_SCOPE();

	map = CreateRef<Map>(Size(5, 5), Size(250, 250));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());

	map->CalculateVisibleChunks(camera->GetPosition());

	TextAsset vsCode("Assets/MapTest.vs");
	TextAsset fsCode("Assets/MapTest.fs");
	shader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_Proj", "u_View"
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

	visibleChunks = map->GetVisibleChunks();
	lastVisibleChunks = visibleChunks;

	int amountOfBlocks = (visibleChunks.x.end - visibleChunks.x.start) * (visibleChunks.y.end - visibleChunks.y.start) * map->GetAmountOfChunks().x * map->GetAmountOfChunks().y;

	vao = CreateRef<Werwel::VAO>();
	vao->BindSafely();
		vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
		vbo = vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, amountOfBlocks, sizeof(Vec4), nullptr, std::vector<Werwel::VertexBufferLayout> { { 4, sizeof(Vec4), 0, 1 } });

	PopulateBlockData();
}

void Engine::PopulateBlockData() {
	FORGIO_PROFILER_SCOPE();

	const auto& chunks = map->GetVisibleChunks();
	const auto& blocks = map->GetBlocks();

	for (int x = chunks.x.start; x < chunks.x.end; x++) {
		for (int y = chunks.y.start; y < chunks.y.end; y++) {
			int firstBlockX = x * map->GetChunkSize().x;
			int lastBlockX = (x + 1) * map->GetChunkSize().x;
			int firstBlockY = y * map->GetChunkSize().y;
			int lastBlockY = (y + 1) * map->GetChunkSize().y;

			map->chunks[x][y].memPos = blocksData.size();

			for (int i = firstBlockX; i < lastBlockX; i++) {
				for (int j = firstBlockY; j < lastBlockY; j++) {
					if (blocks[i][j] != BlockType::Empty) {
						Vec2 tile = Vec2(1, 1) + PickRightAngularTile(blocks, i, j);
						blocksData.emplace_back(i * map->GetBlockSize(), j * map->GetBlockSize(), tile.x, tile.y);
					} else {
						blocksData.emplace_back(0, 0, 0, 0);
					}
				}
			}
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

	Linow::Clear();
}

static void OverrideChunk(MapChunk& oldChunk, MapChunk& newChunk, Ref<Map>& map, Ref<Werwel::VBO>& vbo) { 
	FORGIO_PROFILER_SCOPE();

	const auto& blocks = map->GetBlocks();

	std::vector<Vec4> newBlocks;
	bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
	bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

	for (int x = newChunkBounds.x.start; x < newChunkBounds.x.end; x++) {
		for (int y = newChunkBounds.y.start; y < newChunkBounds.y.end; y++) {
			if (blocks[x][y] != BlockType::Empty) {
				Vec2 tile = Vec2(1, 1) + PickRightAngularTile(blocks, x, y);
				newBlocks.emplace_back(x * map->GetBlockSize(), y * map->GetBlockSize(), tile.x, tile.y);
			} else {
				newBlocks.emplace_back(0, 0, 0, 0);
			}
		}
	}

	vbo->Update(newBlocks, newBlocks.size(), oldChunk.memPos);
	newChunk.memPos = oldChunk.memPos;

	FORGIO_SYNC_GPU();
}

static void RerenderChunk(MapChunk& chunk, Ref<Map>& map, Ref<Werwel::VBO>& vbo) {
	OverrideChunk(chunk, chunk, map, vbo);
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

	if (Input::MouseButtonDown(Button::Left)) {
		BlockSettingData& settingBlock = map->SetBlock(camera->GetPosition(), BlockType::Empty);
		
		if (settingBlock.IsSet()) {
			RerenderChunk(map->chunks[settingBlock.chunk.x][settingBlock.chunk.y], map, vbo);
		}
	}
}

void Engine::OnVisibleChunksChange() {
	FORGIO_PROFILER_SCOPE();

	if (lastVisibleChunks.y.start < visibleChunks.y.start) {
		for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
			int oldChunkIndex = lastVisibleChunks.y.start;
			int newChunkIndex = visibleChunks.y.end - 1;

			auto& oldChunk = map->chunks[x][oldChunkIndex];
			auto& newChunk = map->chunks[x][newChunkIndex];

			OverrideChunk(oldChunk, newChunk, map, vbo);
		}
	} else if (lastVisibleChunks.y.start > visibleChunks.y.start) {
		for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
			int oldChunkIndex = lastVisibleChunks.y.end - 1;
			int newChunkIndex = visibleChunks.y.start;

			auto& oldChunk = map->chunks[x][oldChunkIndex];
			auto& newChunk = map->chunks[x][newChunkIndex];

			OverrideChunk(oldChunk, newChunk, map, vbo);
		}
	}

	if (lastVisibleChunks.x.end < visibleChunks.x.end) {
		for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
			int oldChunkIndex = lastVisibleChunks.x.start;
			int newChunkIndex = visibleChunks.x.end - 1;

			auto& oldChunk = map->chunks[oldChunkIndex][y];
			auto& newChunk = map->chunks[newChunkIndex][y];

			OverrideChunk(oldChunk, newChunk, map, vbo);
		}
	} else if (lastVisibleChunks.x.end > visibleChunks.x.end) {
		for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
			int oldChunkIndex = lastVisibleChunks.x.end - 1;
			int newChunkIndex = visibleChunks.x.start;

			auto& oldChunk = map->chunks[oldChunkIndex][y];
			auto& newChunk = map->chunks[newChunkIndex][y];

			OverrideChunk(oldChunk, newChunk, map, vbo);
		}
	}
}

void Engine::Render() {
	camera->OnPositionChange([&]() {
		map->CalculateVisibleChunks(camera->GetPosition());
		visibleChunks = map->GetVisibleChunks();

		if (lastVisibleChunks != visibleChunks) {
			OnVisibleChunksChange();
			lastVisibleChunks = visibleChunks;
		}
	});

	{
		FORGIO_PROFILER_NAMED_SCOPE("Binding");
		shader->Bind();
		shader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));
		tileMap->BindSafely();
		vao->BindSafely();
		vao->GetIndexBuffer()->BindSafely();
		FORGIO_SYNC_GPU();
	}

  static Color sky = Color(209, 247, 255, 255.0f) / 255.0f;
	Werwel::GraphicsContext::ClearColor(sky.r, sky.g, sky.b, sky.a);
	Werwel::GraphicsContext::Clear();

	{
		FORGIO_PROFILER_NAMED_SCOPE("Rendering");
		glDrawElementsInstanced(GL_TRIANGLES, vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, blocksData.size());
		FORGIO_SYNC_GPU();
	}
	
	Linow::Render(Math::ToPtr(Window::GetSpace()), Math::ToPtr(camera->GetViewMatrix()));

	ImGui::Begin("Info");
		ImGui::Text(("FPS:" + std::to_string(Time::GetFps())).c_str());
	ImGui::End();

	ImGui::Begin("View");
		ImGui::Text(("x: " + std::to_string(visibleChunks.x.start) + " " + std::to_string(visibleChunks.x.end)).c_str());
		ImGui::Text(("y: " + std::to_string(visibleChunks.y.start) + " " + std::to_string(visibleChunks.y.end)).c_str());
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