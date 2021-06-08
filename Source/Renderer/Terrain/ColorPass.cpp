#include "ColorPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"

#include "Werwel/GraphicsContext.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

ColorPass::ColorPass(Ref<Map>& map) {
  this->map = map;
	mapRenderer = CreateRef<MapRenderer>(map);

	const TextAsset chunkShaderVsCode("Assets/Shaders/Terrain/Chunk.vs");
	const TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Chunk.fs");
	shader = CreateRef<Werwel::Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos", "u_ColorPass", "u_Highlight");

	const auto& vertices = Primitives::Pixel::vertices;
	const auto& indices = Primitives::Pixel::indices;

	chunkVao = CreateRef<Werwel::VAO>();
	chunkVao->Bind();
		chunkVao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		chunkVao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
	chunkVao->Unbind();
	// chunkVao = CreateRef<VAO>(Primitives::Pixel::vertices, Vertex::GetLayout(), Primitives::Pixel::indices);
  
	const Mat4 chunkModelMatrix = Math::Scale(Mat4(1), Vec3(mapRenderer->GetChunkSizePixels().x, -mapRenderer->GetChunkSizePixels().y, 1.0f));
	shader->Bind();
		shader->SetInt("u_ColorPass", 0);
		shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	shader->Unbind();

  fbo = CreateRef<ColorFBO>(Window::GetSize());
}

void ColorPass::Execute(const Mat4& viewMatrix, const Vec2& viewPos) {
  FORGIO_PROFILER_SCOPE();

	info.chunksRendered = 0;

	Werwel::GraphicsContext::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	light.clear();

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
      chunkVao->Bind();
			chunkVao->GetIndexBuffer()->Bind();
        const bounds_t& bounds = map->GetVisibleChunks();
        
        for (int x = bounds.x.start; x < bounds.x.end; x++) {
          for (int y = bounds.y.start; y < bounds.y.end; y++) {
						FORGIO_PROFILER_NAMED_SCOPE("foreach chunk (x, y)");

						auto& chunk = mapRenderer->chunks[x][y];
						if (!chunk.containsOnlyEmptyBlocks) {
            	mapRenderer->chunks[x][y].Render(shader);
						}
						
						for (int i = 0; i < mapRenderer->chunks[x][y].GetLightData().size(); i++) {
							light.push_back(mapRenderer->chunks[x][y].GetLightData()[i]);
						}
						
						info.chunksRendered += 1;
          }
        }
        
      chunkVao->Unbind();
    shader->Unbind();
  fbo->Unbind();
}