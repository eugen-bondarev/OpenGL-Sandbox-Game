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
		vbo = vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, 8192, sizeof(Vec4), nullptr, std::vector<Werwel::VertexBufferLayout> { { 4, sizeof(Vec4), 0, 1 } });
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::BeginFrame();
	Gui::BeginFrame();
	Time::BeginFrame();
	Input::BeginFrame();

	debugRenderer->ClearLines();
}

void Engine::Control() {
}

void Engine::Render() {
	Werwel::GraphicsContext::Clear();

	std::vector<Vec4> blocksData;

	const auto& chunks = map->GetVisibleChunks();
	const auto& blocks = map->GetBlocks();

	int blocksRendered = 0;

	{
		FORGIO_PROFILER_NAMED_SCOPE("Building blocks!");
		for (int x = chunks.x.start; x < chunks.x.end; x++) {
			for (int y = chunks.y.start; y < chunks.y.end; y++) {
				int firstBlockX = x * 16.0f;
				int lastBlockX = (x + 1) * 16.0f;
				int firstBlockY = y * 16.0f;
				int lastBlockY = (y + 1) * 16.0f;

				for (int i = firstBlockX; i < lastBlockX; i++) {
					for (int j = firstBlockY; j < lastBlockY; j++) {
						if (blocks[i][j] != BlockType::Empty) {
							blocksData.emplace_back(i * 16.0f, j * 16.0f, 1, 0);
						}
					}
				}
			}
		}
	}

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
		FORGIO_PROFILER_NAMED_SCOPE("Updating buffer");
		vbo->Update(blocksData, blocksData.size());
		glFinish();
	}

	{
		FORGIO_PROFILER_NAMED_SCOPE("Rendering!");
		glDrawElementsInstanced(GL_TRIANGLES, vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, blocksData.size());
		glFinish();
	}

	ImGui::Begin("Info");
	ImGui::End();

	ImGui::Begin("View");
		auto& visibleChunks = map->GetVisibleChunks();
		ImGui::Text(("x: " + std::to_string(visibleChunks.x.start) + " " + std::to_string(visibleChunks.x.end)).c_str());
		ImGui::Text(("y: " + std::to_string(visibleChunks.y.start) + " " + std::to_string(visibleChunks.y.end)).c_str());
		ImGui::Text(("Blocks rendered: " + std::to_string(blocksRendered)).c_str());
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