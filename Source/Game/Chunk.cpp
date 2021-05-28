#include "Chunk.h"

#include "Core/Window.h"

Chunk::Chunk(Pos chunkPos, Size chunkSize, ChunkFbo** fbo, Shader* shader, Vao* vao, Texture* tileMap, chunk_t chunk, blocks_t* blocks) {
  this->chunkPos = chunkPos;
  this->chunkSize = chunkSize;
  this->fbo = fbo;
  this->shader = shader;
  this->vao = vao;
  this->tileMap = tileMap;
  this->chunk = chunk;
  this->blocks = blocks;

  texture = std::make_unique<Texture>(
    Size{192, 192},
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

void Chunk::Prepare() {
  *fbo = new ChunkFbo(texture);

	viewPos = (chunkPos * chunkSize) * BLOCK_SIZE;
	viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));
}

void Chunk::Rerender() {
	glViewport(0.0f, 0.0f, 192, 192);

  (*fbo)->Bind();
  (*fbo)->Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	vao->Bind();
    		tileMap->Bind();
          for (int x = chunk.x.start; x < chunk.x.end; x++) {
            for (int y = chunk.y.start; y < chunk.y.end; y++) {
              BlockType type = (*blocks)[x][y];

              if (type == BlockType::Empty) continue;

              Vec2 textureOffset = Vec2(0, 0);

              switch (type) {
                case BlockType::Dirt:
                  textureOffset = Vec2(0, 0);
                  break;
                case BlockType::Grass:
                  textureOffset = Vec2(1, 1);
                  break;
              }

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
  (*fbo)->Unbind();

  delete *fbo;

	glViewport(0.0f, 0.0f, Window::GetSize().x, Window::GetSize().y);
}