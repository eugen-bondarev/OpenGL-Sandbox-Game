#include "Chunk.h"

#include "Core/Window.h"

#include "Gpu/GraphicsContext.h"

Chunk::Chunk(
  Pos chunkPos, 
  Size chunkSize, 
  std::shared_ptr<Shader> shader, 
  std::shared_ptr<Shader> lightShader, 
  std::shared_ptr<Vao> vao, 
  std::shared_ptr<Vao> lightVao, 
  std::shared_ptr<Texture> tileMap, 
  std::shared_ptr<Texture> lightTexture, 
  bounds_t bounds, 
  blocks_t& blocksReference
) : blocks { blocksReference } {
  this->chunkPos = chunkPos;
  this->chunkSize = chunkSize;
  this->shader = shader;
  this->lightShader = lightShader;
  this->vao = vao;
  this->lightVao = lightVao;
  this->tileMapTexture = tileMap;
  this->lightTexture = lightTexture;
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

  targetLightTexture = std::make_unique<Texture>(
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

  light.clear();

	GraphicsContext::ClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });

  fbo.Bind();
  fbo.Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	vao->Bind();
    		tileMapTexture->Bind();
          for (int x = bounds.x.start - 5; x < bounds.x.end + 5; x++) {
            for (int y = bounds.y.start - 5; y < bounds.y.end + 5; y++) {
              if (x < 0 || x >= blocks.size() || y < 0 || y >= blocks.size()) {
                continue;
              }

              if (blocks[x][y] == BlockType::Empty) {
                if (y > 0 && blocks[x][y - 1] != BlockType::Empty) {
                  light.emplace_back(x, y);
                }
              }
            }
          }

          for (int x = bounds.x.start; x < bounds.x.end; x++) {
            for (int y = bounds.y.start; y < bounds.y.end; y++) {
              const BlockType type = blocks[x][y];

              if (type == BlockType::Empty) {
                continue;
              }

              Vec2 textureOffset = Vec2(1, 1);

              textureOffset += tileMapDictionary[type];

              const Vec2 pos = Vec2(x * BLOCK_SIZE, y * BLOCK_SIZE);
              const Vec2 chunkCenter = chunkSize / 2.0f * BLOCK_SIZE - BLOCK_SIZE / 2.0f;
              const Vec2 shiftedPosition = pos - chunkCenter;
              shader->SetVec2("u_Tile", Math::ToPtr(textureOffset));
              shader->SetVec2("u_Pos", Math::ToPtr(shiftedPosition));
              glDrawElements(GL_TRIANGLES, vao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            }
          }
    		tileMapTexture->Unbind();
    	vao->Unbind();
    shader->Unbind();
  fbo.Unbind();

  LightFbo lightFbo(targetLightTexture);

  lightFbo.Bind();
  lightFbo.Clear();
    lightShader->Bind();
    lightShader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	lightVao->Bind();
        lightTexture->Bind();
          for (int i = 0; i < light.size(); i++) {
            const Vec2 pos = light[i] * BLOCK_SIZE;
            const Vec2 chunkCenter = chunkSize / 2.0f * BLOCK_SIZE - BLOCK_SIZE / 2.0f;
            const Vec2 shiftedPosition = pos - chunkCenter;
            lightShader->SetVec2("u_Pos", Math::ToPtr(shiftedPosition));
            glDrawElements(GL_TRIANGLES, lightVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
          }
        lightTexture->Unbind();
    	lightVao->Unbind();
    lightShader->Unbind();
  lightFbo.Unbind();

	glViewport(0.0f, 0.0f, Window::GetSize().x, Window::GetSize().y);
}

void Chunk::Render(std::shared_ptr<Shader>& shader) {
  targetTexture->Bind(GL_TEXTURE0);
  targetLightTexture->Bind(GL_TEXTURE0 + 1);
  // lightTexture->Bind();
    const Vec2 chunkPosPixels = chunkPos * chunkSize * BLOCK_SIZE;
    Mat4 chunkModelMatrix = Math::Translate(Mat4(1), Vec3(chunkPosPixels, 1.0f));
    chunkModelMatrix = Math::Scale(chunkModelMatrix, Vec3((chunkSize * BLOCK_SIZE).x, -(chunkSize * BLOCK_SIZE).y, 1.0f));
    shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
  targetLightTexture->Unbind(GL_TEXTURE0 + 1);
  targetTexture->Unbind();
  // lightTexture->Unbind();
}