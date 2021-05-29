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
	shader = std::make_shared<Shader>(chunkShaderVsCode.GetContent(), chunkShaderFsCode.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos", "u_ColorPass", "u_LightPass");
	chunkVao = std::make_shared<Vao>(Primitives::Pixel::vertices, Vertex::GetLayout(), Primitives::Pixel::indices);
  
	const Mat4 chunkModelMatrix = Math::Scale(Mat4(1), Vec3(mapRenderer->GetChunkSizePixels().x, -mapRenderer->GetChunkSizePixels().y, 1.0f));
	shader->Bind();
		shader->SetInt("u_ColorPass", 0);
		shader->SetInt("u_LightPass", 1);
		shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
		shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	shader->Unbind();

  fbo = std::make_shared<ColorFbo>(Window::GetSize());
}

void ColorPass::Execute(const Mat4& viewMatrix, const Vec2& viewPos) {
	info.chunksRendered = 0;

	GraphicsContext::ClearColor({ 224 / 255.0f, 236 / 255.0f, 255 / 255.0f, 1.0f });

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
      chunkVao->Bind();
        bounds_t bounds = GetVisibleChunks(map, viewPos);
        
        for (int x = bounds.x.start; x < bounds.x.end; x++) {
          for (int y = bounds.y.start; y < bounds.y.end; y++) {
            mapRenderer->chunks[x][y].Render(shader);
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

	return bounds;
}