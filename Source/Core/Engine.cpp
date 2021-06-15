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

void Engine::InitPipeline() {
	InitColorPass();
	InitLightPass();
	InitCompositionPass();
}

void Engine::InitColorPass() {
	pipeline.colorPass = CreateRef<ColorPass>(visibleChunks.GetArea() * map->GetAmountOfChunks().x * map->GetAmountOfChunks().y);
}

void Engine::InitLightPass() {
	pipeline.lightPass.fbo = CreateRef<LightFBO>(Window::GetSize());

	TextAsset vsCode("Assets/Shaders/Terrain/LightPassShader.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/LightPassShader.fs");
	pipeline.lightPass.shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View");
	
	const auto& vertices = Primitives::Block::Vertices(256, 256);
	const auto& indices = Primitives::Block::indices;

	pipeline.lightPass.lightMesh.vao = CreateRef<Werwel::VAO>();
	pipeline.lightPass.lightMesh.vao->Bind();
		pipeline.lightPass.lightMesh.vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		pipeline.lightPass.lightMesh.vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
		pipeline.lightPass.lightMesh.dynamicVBO = pipeline.lightPass.lightMesh.vao->AddVBO(
			Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, 10000, sizeof(Vec2), nullptr,
			Werwel::VertexBufferLayouts { { 2, sizeof(Vec2), 0, 1 } }
		);


	ImageAsset lightTexture("Assets/Images/LightMask.png");
	pipeline.lightPass.lightMesh.texture = CreateRef<Werwel::Texture>(
		Werwel::Size { lightTexture.GetSize().x, lightTexture.GetSize().y },
		lightTexture.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE }
	);
}

void Engine::LightPass() {
// No culling of back faces
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	glClearColor(0, 0, 0, 1);

	pipeline.lightPass.fbo->Bind();
	pipeline.lightPass.fbo->Clear();
		pipeline.lightPass.shader->Bind();
		pipeline.lightPass.shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
		pipeline.lightPass.shader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));
			pipeline.lightPass.lightMesh.vao->Bind();
			pipeline.lightPass.lightMesh.vao->GetIndexBuffer()->Bind();
				pipeline.lightPass.lightMesh.texture->Bind();
					glDrawElementsInstanced(GL_TRIANGLES, pipeline.colorPass->vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, 12000);
					
		pipeline.lightPass.shader->Unbind();
	pipeline.lightPass.fbo->Unbind();
}

void Engine::InitCompositionPass() {
	TextAsset vsCode("Assets/Shaders/Terrain/CompositionShader.vs");
	TextAsset fsCode("Assets/Shaders/Terrain/CompositionShader.fs");
	pipeline.compositionPass.shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ColorPassResult", "u_LightPassResult");
	pipeline.compositionPass.shader->Bind();
		pipeline.compositionPass.shader->SetInt("u_ColorPassResult", 0);
		pipeline.compositionPass.shader->SetInt("u_LightPassResult", 1);
	pipeline.compositionPass.shader->Unbind();
	
	const auto& vers = Primitives::Block::Vertices(2, -2);
	const auto& inds = Primitives::Block::indices;

	pipeline.compositionPass.canvas = CreateRef<Werwel::VAO>();
	pipeline.compositionPass.canvas->BindSafely();
	pipeline.compositionPass.canvas->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
	pipeline.compositionPass.canvas->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
}

void Engine::Compose() {
	pipeline.compositionPass.shader->Bind();

	pipeline.compositionPass.canvas->Bind();
	pipeline.compositionPass.canvas->GetIndexBuffer()->Bind();

	pipeline.colorPass->GetFBO()->GetTexture()->Bind(GL_TEXTURE0);
	pipeline.lightPass.fbo->GetTexture()->Bind(GL_TEXTURE0 + 1);

		glDrawElements(GL_TRIANGLES, pipeline.compositionPass.canvas->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

	glActiveTexture(GL_TEXTURE0);
}

void Engine::InitResources() {
	FORGIO_PROFILER_SCOPE();

	map = CreateRef<Map>(Size(5, 5), Size(250, 250));
	camera = CreateRef<Camera>();
	camera->SetPosition(map->GetCenter() * map->GetBlockSize());

	map->CalculateVisibleChunks(camera->GetPosition());

	visibleChunks = map->GetVisibleChunks();
	lastVisibleChunks = visibleChunks;

	InitPipeline();

	PopulateBlockData();
}

static void ProcessBlock(const Ref<Map>& map, Vec4& block, Vec2& light, int x, int y) {
	if (map->GetBlocks()[x][y] != BlockType::Empty) {
		const Vec2 tile = Vec2(1, 1) + PickRightAngularTile(map->GetBlocks(), x, y);
		block = Vec4(x * map->GetBlockSize(), y * map->GetBlockSize(), tile.x, tile.y);
	} else {
		if (map->GetBlocks()[x][y - 1] != BlockType::Empty) {
			light = Vec2(16 * x, 16 * y);
		}
	}
}

void Engine::PopulateBlockData() {
	FORGIO_PROFILER_SCOPE();

	std::vector<Vec4> blocksData;
	std::vector<Vec2> lightData;

	const auto& chunks = map->GetVisibleChunks();
	const auto& blocks = map->GetBlocks();

	for (int i = chunks.x.start; i < chunks.x.end; i++) {
		for (int j = chunks.y.start; j < chunks.y.end; j++) {
			const bounds_t chunk = {
				{ i * static_cast<int>(map->GetChunkSize().x), (i + 1) * static_cast<int>(map->GetChunkSize().x) },
				{ j * static_cast<int>(map->GetChunkSize().y), (j + 1) * static_cast<int>(map->GetChunkSize().y) }
			};

			map->chunks[i][j].colorMemOffset = blocksData.size();
			map->chunks[i][j].lightMemOffset = lightData.size();

			for (int x = chunk.x.start; x < chunk.x.end; x++) {
				for (int y = chunk.y.start; y < chunk.y.end; y++) {
					Vec4& newBlock = blocksData.emplace_back(0);
					Vec2& newLightBlock = lightData.emplace_back(0);
					ProcessBlock(map, newBlock, newLightBlock, x, y);
				}
			}
		}
	}

	pipeline.colorPass->vbo->Update(blocksData, blocksData.size());
	pipeline.lightPass.lightMesh.dynamicVBO->Update(lightData, lightData.size());
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

static void OverrideChunk(MapChunk& oldChunk, MapChunk& newChunk, Ref<Map>& map, Ref<Werwel::VBO>& vbo, Ref<Werwel::VBO>& lightVBO) { 
	FORGIO_PROFILER_SCOPE();

	const auto& blocks = map->GetBlocks();

	std::vector<Vec4> newBlocks;
	std::vector<Vec2> lightBlocks;
	bounds_t oldChunkBounds = map->WhatBlocks(oldChunk.index);
	bounds_t newChunkBounds = map->WhatBlocks(newChunk.index);

	for (int x = newChunkBounds.x.start; x < newChunkBounds.x.end; x++) {
		for (int y = newChunkBounds.y.start; y < newChunkBounds.y.end; y++) {
			Vec4& newBlock = newBlocks.emplace_back(0);
			Vec2& newLightBlock = lightBlocks.emplace_back(0);
			ProcessBlock(map, newBlock, newLightBlock, x, y);
		}
	}

	vbo->Update(newBlocks, newBlocks.size(), oldChunk.colorMemOffset);
	lightVBO->Update(lightBlocks, lightBlocks.size(), oldChunk.lightMemOffset);

	newChunk.colorMemOffset = oldChunk.colorMemOffset;
	newChunk.lightMemOffset = oldChunk.lightMemOffset;

	FORGIO_SYNC_GPU();
}

static void RerenderChunk(MapChunk& chunk, Ref<Map>& map, Ref<Werwel::VBO>& vbo, Ref<Werwel::VBO>& lightVBO) {
	OverrideChunk(chunk, chunk, map, vbo, lightVBO);
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
			RerenderChunk(map->chunks[settingBlock.chunk.x][settingBlock.chunk.y], map, pipeline.colorPass->vbo, pipeline.lightPass.lightMesh.dynamicVBO);
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		BlockSettingData& settingBlock = map->SetBlock(camera->GetPosition(), BlockType::Dirt);

		if (settingBlock.IsSet()) {
			RerenderChunk(map->chunks[settingBlock.chunk.x][settingBlock.chunk.y], map, pipeline.colorPass->vbo, pipeline.lightPass.lightMesh.dynamicVBO);
		}
	}
}

void Engine::OnVisibleChunksChange() {
	FORGIO_PROFILER_SCOPE();

	if (lastVisibleChunks.y.start < visibleChunks.y.start) {
		for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
			OverrideChunk(map->chunks[x][lastVisibleChunks.y.start], map->chunks[x][visibleChunks.y.end - 1], map, pipeline.colorPass->vbo, pipeline.lightPass.lightMesh.dynamicVBO);
		}
	} else if (lastVisibleChunks.y.start > visibleChunks.y.start) {
		for (int x = lastVisibleChunks.x.start; x < lastVisibleChunks.x.end; x++) {
			OverrideChunk(map->chunks[x][lastVisibleChunks.y.end - 1], map->chunks[x][visibleChunks.y.start], map, pipeline.colorPass->vbo, pipeline.lightPass.lightMesh.dynamicVBO);
		}
	}

	if (lastVisibleChunks.x.end < visibleChunks.x.end) {
		for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
			OverrideChunk(map->chunks[lastVisibleChunks.x.start][y], map->chunks[visibleChunks.x.end - 1][y], map, pipeline.colorPass->vbo, pipeline.lightPass.lightMesh.dynamicVBO);
		}
	} else if (lastVisibleChunks.x.end > visibleChunks.x.end) {
		for (int y = lastVisibleChunks.y.start; y < lastVisibleChunks.y.end; y++) {
			OverrideChunk(map->chunks[lastVisibleChunks.x.end - 1][y], map->chunks[visibleChunks.x.start][y], map, pipeline.colorPass->vbo, pipeline.lightPass.lightMesh.dynamicVBO);
		}
	}
}

void Engine::Render() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

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
		pipeline.colorPass->shader->Bind();
		pipeline.colorPass->shader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));
		pipeline.colorPass->tileMap->BindSafely();
		pipeline.colorPass->vao->BindSafely();
		pipeline.colorPass->vao->GetIndexBuffer()->BindSafely();
		FORGIO_SYNC_GPU();
	}
	glClearColor(0, 0, 0, 0);

	{
		FORGIO_PROFILER_NAMED_SCOPE("Rendering");

			pipeline.colorPass->GetFBO()->Bind();
			pipeline.colorPass->GetFBO()->Clear();
				const int amountOfBlocks = visibleChunks.GetArea() * map->GetChunkSize().x * map->GetChunkSize().y;
				glDrawElementsInstanced(GL_TRIANGLES, pipeline.colorPass->vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfBlocks);
			pipeline.colorPass->GetFBO()->Unbind();

		FORGIO_SYNC_GPU();
	}

	LightPass();
	

  static Color sky = Color(209, 247, 255, 255.0f) / 255.0f;
	Werwel::GraphicsContext::ClearColor(sky.r, sky.g, sky.b, sky.a);
	Werwel::GraphicsContext::Clear();
	
	Compose();

	// ImGui::MyImage("Image", pipeline.lightPass.fbo->GetTexture(), ImVec2(800, 600));
	
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