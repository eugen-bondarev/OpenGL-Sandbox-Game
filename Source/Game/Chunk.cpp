#include "Chunk.h"

#include "Core/Window.h"

#include "GPU/GraphicsContext.h"

#include "Renderer/Terrain/Resources/ChunkFBO.h"
#include "Renderer/Terrain/Resources/LightFBO.h"

Chunk::Chunk(
  Pos chunkPos, 
  Size chunkSize, 

  Ref<Shader> shader, 
  Ref<VAO> vao, 
  Ref<Texture> tileMap, 
  
  bounds_t bounds, 
  blocks_t& blocksReference,
  float blockSize
) : blocks { blocksReference } {
  this->chunkPos = chunkPos;
  this->chunkSize = chunkSize;
  this->shader = shader;
  this->vao = vao;
  this->tileMapTexture = tileMap;
  this->bounds = bounds;
  this->blockSize = blockSize;

  targetTexture = CreateRef<Texture>(
    chunkSize * blockSize,
    nullptr,
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
    Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
  );
}

bool TopBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return y < blocks[x].size() - 1 && blocks[x][y + 1] == BlockType::Empty;
}

bool LeftBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return x > 0 && blocks[x - 1][y] == BlockType::Empty;
}

bool BottomBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return y > 0 && blocks[x][y - 1] == BlockType::Empty;
}

bool RightBlockIsEmpty(const blocks_t& blocks, int x, int y) {
  return x < blocks.size() - 1 && blocks[x + 1][y] == BlockType::Empty;
}

Vec2 PickRightAngularTile(const blocks_t& blocks, int x, int y) {
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

void Chunk::Rerender() {
  const ChunkFBO fbo(targetTexture);

	const Vec2 viewPos = (chunkPos * chunkSize) * blockSize;
	const Mat4 viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

  GraphicsContext::Viewport(0, 0, chunkSize.x * blockSize, chunkSize.y * blockSize);
	GraphicsContext::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  lightData.clear();

  fbo.Bind();
  fbo.Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	vao->Bind();
      vao->GetIndexBuffer()->Bind();
    		tileMapTexture->Bind();
          for (int x = bounds.x.start; x < bounds.x.end; x++) {
            for (int y = bounds.y.start; y < bounds.y.end; y++) {
              const BlockType type = blocks[x][y];

              if (type == BlockType::Empty) {
                if (y + 1 < blocks[x].size() && y > 0 && blocks[x][y - 1] != BlockType::Empty) {
                  lightData.emplace_back((x - chunkSize.x / 2 + 0.5f) * blockSize, (y - chunkSize.y / 2 + 0.5f) * blockSize);
                }

                continue;
              }

              Vec2 textureOffset = Vec2(0, 0);

              textureOffset += tileMapDictionary[type];

              Vec2 additionalOffset = PickRightAngularTile(blocks, x, y);              

              textureOffset += additionalOffset;

              const Vec2 pos = Vec2(x * blockSize, y * blockSize);
              const Vec2 chunkCenter = chunkSize / 2.0f * blockSize - blockSize / 2.0f;
              shader->SetVec2("u_Tile", Math::ToPtr(textureOffset));
              shader->SetVec2("u_Pos", Math::ToPtr(pos - chunkCenter));
              glDrawElements(GL_TRIANGLES, vao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            }
          }
    		tileMapTexture->Unbind();
    	vao->Unbind();
    shader->Unbind();
  fbo.Unbind();

	GraphicsContext::Viewport(0, 0, Window::GetSize().x, Window::GetSize().y);
}

void Chunk::Render(std::shared_ptr<Shader>& shader) {
  targetTexture->Bind();
    const Vec2 chunkPosPixels = chunkPos * chunkSize * blockSize;
    Mat4 chunkModelMatrix = Math::Translate(Mat4(1), Vec3(chunkPosPixels, 1.0f));
    chunkModelMatrix = Math::Scale(chunkModelMatrix, Vec3((chunkSize * blockSize).x, -(chunkSize * blockSize).y, 1.0f));
    shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, nullptr);
  targetTexture->Unbind();
}