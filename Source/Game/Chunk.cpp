#include "Chunk.h"

#include "Core/Window.h"

#include "Gpu/GraphicsContext.h"

Chunk::Chunk(
  Pos chunkPos, 
  Size chunkSize, 
  std::shared_ptr<Shader> shader, 
  std::shared_ptr<Vao> vao, 
  std::shared_ptr<Texture> tileMap, 
  bounds_t bounds, 
  blocks_t& blocksReference
) : blocks { blocksReference } {
  this->chunkPos = chunkPos;
  this->chunkSize = chunkSize;
  this->shader = shader;
  this->vao = vao;
  this->tileMapTexture = tileMap;
  this->bounds = bounds;

  targetTexture = std::make_unique<Texture>(
    chunkSize * BLOCK_SIZE,
    nullptr,
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    std::vector<Texture::param_t>{
      {ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST},
      {ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST},
    }
  );
}

void Chunk::Rerender() {
  ChunkFbo fbo(targetTexture);

	const Vec2 viewPos = (chunkPos * chunkSize) * BLOCK_SIZE;
	const Mat4 viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

	glViewport(0.0f, 0.0f, chunkSize.x * BLOCK_SIZE, chunkSize.y * BLOCK_SIZE);

	GraphicsContext::ClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });

  lightData.clear();

  fbo.Bind();
  fbo.Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	vao->Bind();
    		tileMapTexture->Bind();
          for (int x = bounds.x.start; x < bounds.x.end; x++) {
            for (int y = bounds.y.start; y < bounds.y.end; y++) {
              const BlockType type = blocks[x][y];

              if (type == BlockType::Empty) {
                if (y + 1 < blocks[x].size() && y > 0 && blocks[x][y - 1] != BlockType::Empty) {
                  lightData.emplace_back((x - chunkSize.x / 2) * BLOCK_SIZE, (y - chunkSize.y / 2) * BLOCK_SIZE);
                }

                continue;
              }

              Vec2 textureOffset = Vec2(1, 1);

              textureOffset += tileMapDictionary[type];

              const Vec2 pos = Vec2(x * BLOCK_SIZE, y * BLOCK_SIZE);
              const Vec2 chunkCenter = chunkSize / 2.0f * BLOCK_SIZE - BLOCK_SIZE / 2.0f;
              shader->SetVec2("u_Tile", Math::ToPtr(textureOffset));
              shader->SetVec2("u_Pos", Math::ToPtr(pos - chunkCenter));
              glDrawElements(GL_TRIANGLES, vao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            }
          }
    		tileMapTexture->Unbind();
    	vao->Unbind();
    shader->Unbind();
  fbo.Unbind();

	glViewport(0.0f, 0.0f, Window::GetSize().x, Window::GetSize().y);
}

void Chunk::Render(std::shared_ptr<Shader>& shader) {
  targetTexture->Bind();
    const Vec2 chunkPosPixels = chunkPos * chunkSize * BLOCK_SIZE;
    Mat4 chunkModelMatrix = Math::Translate(Mat4(1), Vec3(chunkPosPixels, 1.0f));
    chunkModelMatrix = Math::Scale(chunkModelMatrix, Vec3((chunkSize * BLOCK_SIZE).x, -(chunkSize * BLOCK_SIZE).y, 1.0f));
    shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  targetTexture->Unbind();
}