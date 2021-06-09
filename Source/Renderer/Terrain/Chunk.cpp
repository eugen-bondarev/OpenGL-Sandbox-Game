#include "Chunk.h"

#include "Core/Window.h"

#include "Werwel/GraphicsContext.h"

#include "Renderer/Terrain/Resources/ChunkFBO.h"
#include "Renderer/Terrain/Resources/LightFBO.h"

#include "Util/Vector.h"

Chunk::Chunk(
  Pos chunkPos, 
  Size chunkSize,  
  bounds_t bounds, 
  float blockSize
) {
  this->chunkPos = chunkPos;
  this->chunkSize = chunkSize;
  this->bounds = bounds;
  this->blockSize = blockSize;

  Size targetTextureSize = chunkSize * blockSize;

  targetTexture = CreateRef<Werwel::Texture>(
    Werwel::Size(targetTextureSize.x, targetTextureSize.y),
    nullptr,
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
  );

  const Vec2 chunkPosPixels = chunkPos * chunkSize * blockSize;
  chunkModelMatrix = Math::Translate(Mat4(1), Vec3(chunkPosPixels, 1.0f));
  chunkModelMatrix = Math::Scale(chunkModelMatrix, Vec3((chunkSize * blockSize).x, -(chunkSize * blockSize).y, 1.0f));
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

void Chunk::AddWallToRenderingData(std::vector<Vec4>& wallData, WallType wall, const walls_t& walls, int x, int y) {
  const Vec2 textureOffset = wallsTileMapDictionary[wall] + PickRightAngularTile(walls, x, y);
  const Vec2 pos = Vec2(x * blockSize, y * blockSize);
  const Vec2 chunkCenter = chunkSize / 2.0f * blockSize - blockSize / 2.0f;
  static const Vec2 wallOffset = Vec2(4, -4);
  const Vec2 finalPos = pos - chunkCenter + wallOffset;

  wallData.emplace_back(finalPos.x, finalPos.y, textureOffset.x, textureOffset.y);
}

void Chunk::AddBlockToRenderingData(std::vector<Vec4>& blockData, BlockType block, const blocks_t& blocks, int x, int y) {  
  const Vec2 textureOffset = tileMapDictionary[block] + PickRightAngularTile(blocks, x, y);
  const Vec2 pos = Vec2(x * blockSize, y * blockSize);
  const Vec2 chunkCenter = chunkSize / 2.0f * blockSize - blockSize / 2.0f;
  const Vec2 finalPos = pos - chunkCenter;

  blockData.emplace_back(finalPos.x, finalPos.y, textureOffset.x, textureOffset.y);
}

void Chunk::Rerender(  
  Ref<Werwel::Shader>& shader, 
  Ref<Werwel::VAO>& chunkVAO, 
  Ref<Werwel::VBO>& positionAndTileVBO, 
  Ref<Werwel::Texture>& tileMapTexture,
  const blocks_t& blocks,
  const walls_t& walls
) {
  FORGIO_PROFILER_SCOPE();
  
  const ChunkFBO fbo(targetTexture);

	const Vec2 viewPos = (chunkPos * chunkSize) * blockSize;
	const Mat4 viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

  Werwel::GraphicsContext::Viewport(0, 0, chunkSize.x * blockSize, chunkSize.y * blockSize);
	Werwel::GraphicsContext::ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  lightData.clear();

  containsOnlyEmptyBlocks = true;

  fbo.Bind();
  fbo.Clear();
    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
    	chunkVAO->Bind();
      chunkVAO->GetIndexBuffer()->Bind();
    		tileMapTexture->Bind();
          std::vector<Vec4> blocksData;
          std::vector<Vec4> wallsData;

          Period<> xAxis = { 
            std::max<int>(bounds.x.start - 1, 0), 
            std::min<int>(bounds.x.end + 1, blocks.size() - 1)
          };
          
          Period<> yAxis = { 
            std::max<int>(bounds.y.start - 1, 0), 
            std::min<int>(bounds.y.end + 1, blocks[0].size() - 1)
          };

          for (int x = xAxis.start; x < xAxis.end; x++) {
            for (int y = yAxis.start; y < yAxis.end; y++) {
              const BlockType blockType = blocks[x][y];
              const WallType wallType = walls[x][y];

              bool isNormalWall { false };

              if (blockType == BlockType::Empty) {
                if (wallType == WallType::Empty) {
                  if (blocks[x][y - 1] != BlockType::Empty || walls[x][y - 1] != BlockType::Empty) {
                    lightData.emplace_back((x - chunkSize.x / 2 + 0.5f) * blockSize, (y - chunkSize.y / 2 + 0.5f) * blockSize);
                  }
                  continue;
                } else {
                  isNormalWall = true;
                }
              }
              
              if (!isNormalWall) {
                AddBlockToRenderingData(blocksData, blockType, blocks, x, y);
                containsOnlyEmptyBlocks = false;

                bool insideBounds = x > 0 && x < blocks.size() - 1 && y > 0 && y < blocks[x].size() - 1;

                if (insideBounds && wallType != WallType::Empty) {
                  if (blocks[x + 1][y] == BlockType::Empty || blocks[x - 1][y] == BlockType::Empty || blocks[x][y + 1] == BlockType::Empty || blocks[x][y - 1] == BlockType::Empty || blocks[x + 1][y - 1] == BlockType::Empty) {
                    AddWallToRenderingData(wallsData, wallType, walls, x, y);
                    containsOnlyEmptyBlocks = false;
                  }
                }
              } else {
                AddWallToRenderingData(wallsData, wallType, walls, x, y);
                containsOnlyEmptyBlocks = false;
              }
            }
          }

          std::vector<Vec4> joinedRenderingData;
          JoinVectors(wallsData, blocksData, joinedRenderingData);
        
          positionAndTileVBO->Update(joinedRenderingData, joinedRenderingData.size());          
					glDrawElementsInstanced(GL_TRIANGLES, chunkVAO->GetVertexCount(), GL_UNSIGNED_INT, nullptr, joinedRenderingData.size());

    		tileMapTexture->Unbind();
    	chunkVAO->Unbind();
    shader->Unbind();
  fbo.Unbind();

	Werwel::GraphicsContext::Viewport(0, 0, Window::GetSize().x, Window::GetSize().y);
}

void Chunk::Render(Ref<Werwel::Shader>& shader) {
  targetTexture->Bind();
    shader->SetMat4x4("u_Model", Math::ToPtr(chunkModelMatrix));
    glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, nullptr);
  targetTexture->Unbind();
}