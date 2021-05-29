#include "Chunk.h"

#include "Core/Window.h"

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
  this->tileMap = tileMap;
  this->bounds = bounds;

  texture = std::make_unique<Texture>(
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
  ChunkFbo fbo(texture);

	const Vec2 viewPos = (chunkPos * chunkSize) * BLOCK_SIZE;
	const Mat4 viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

	glViewport(0.0f, 0.0f, chunkSize.x * BLOCK_SIZE, chunkSize.y * BLOCK_SIZE);

  fbo.Bind();
  fbo.Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	vao->Bind();
    		tileMap->Bind();
          for (int x = bounds.x.start; x < bounds.x.end; x++) {
            for (int y = bounds.y.start; y < bounds.y.end; y++) {
              BlockType type = blocks[x][y];

              if (type == BlockType::Empty) continue;

              Vec2 textureOffset = Vec2(1, 1);

              textureOffset += tileMapDictionary[type];

              Vec2 pos = Vec2(x * BLOCK_SIZE, y * BLOCK_SIZE);
              Vec2 chunkCenter = chunkSize / 2.0f * BLOCK_SIZE - BLOCK_SIZE / 2.0f;
              Vec2 shiftedPosition = pos - chunkCenter;
              shader->SetVec2("u_Tile", Math::ToPtr(textureOffset));
              shader->SetVec2("u_Pos", Math::ToPtr(shiftedPosition));
              glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            }
          }
    		tileMap->Unbind();
    	vao->Unbind();
    shader->Unbind();
  fbo.Unbind();

	glViewport(0.0f, 0.0f, Window::GetSize().x, Window::GetSize().y);
}