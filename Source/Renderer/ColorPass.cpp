#include "ColorPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"

#include "Gpu/GraphicsContext.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

ColorPass::ColorPass(std::shared_ptr<Map> map) {
  this->map = map;
	mapRenderer = std::make_shared<MapRenderer>(map);

	const TextAsset chunkShaderVsCode("Assets/Shaders/Terrain/Chunk.vs");
	const TextAsset chunkShaderFsCode("Assets/Shaders/Terrain/Chunk.fs");
	shader = std::make_shared<Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos", "u_ColorPass");
	chunkVao = std::make_shared<Vao>(Primitives::Pixel::vertices, Vertex::GetLayout(), Primitives::Pixel::indices);
  
	const Mat4 chunkModelMatrix = Math::Scale(Mat4(1), Vec3(mapRenderer->GetChunkSizePixels().x, -mapRenderer->GetChunkSizePixels().y, 1.0f));
	shader->Bind();
		shader->SetInt("u_ColorPass", 0);
		shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	shader->Unbind();

  fbo = std::make_shared<ColorFbo>(Window::GetSize());
}

void ColorPass::Execute(const Mat4& viewMatrix, const Vec2& viewPos) {
	info.chunksRendered = 0;

	GraphicsContext::ClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });

	light.clear();

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
      chunkVao->Bind();
        bounds_t bounds = GetVisibleChunks(map, viewPos);
        
        for (int x = bounds.x.start; x < bounds.x.end; x++) {
          for (int y = bounds.y.start; y < bounds.y.end; y++) {
            mapRenderer->chunks[x][y].Render(shader);
						
						for (int i = 0; i < mapRenderer->chunks[x][y].lightData.size(); i++) {
							light.push_back(mapRenderer->chunks[x][y].lightData[i]);
						}
						
						info.chunksRendered += 1;
          }
        }
        
      chunkVao->Unbind();
    shader->Unbind();
  fbo->Unbind();
}

bounds_t ColorPass::GetVisibleChunks(std::shared_ptr<Map>& map, Pos viewPos) {
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

	bounds.x.start = std::max(bounds.x.start, 0);
	bounds.y.start = std::max(bounds.y.start, 0);

	bounds.x.end = std::min(bounds.x.end, static_cast<int>(map->GetAmountOfChunks().x) - 1);
	bounds.y.end = std::min(bounds.y.end, static_cast<int>(map->GetAmountOfChunks().y) - 1);

	return bounds;
}