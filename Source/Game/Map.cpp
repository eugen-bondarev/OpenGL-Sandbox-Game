#include "Map.h"

#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"
#include "Core/Window.h"

Map::Map(Size size, int blockSizeInPixels) {
    this->size = size;
    this->blockSizeInPixels = blockSizeInPixels;

    ImageAsset image("Assets/Images/Map1.png");
    tileMap = std::make_shared<Texture>(
        image.GetSize(),
        image.GetData(),
        GL_RGBA,
        image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        std::vector<Texture::param_t> {
            { ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
            { ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
        }
    );

    quadVao = std::make_shared<Vao>(Primitives::Quad::vertices, Vertex::GetLayout(), Primitives::Quad::indices);

    GenerateMap(blocks, size);
    middleOfMap = Vec2(size.x, size.y) / 2.0f;
}

LightData Map::Render(std::shared_ptr<Shader>& shader, Vec2 viewPos) {
    LightData lightData;

    Vec2 cameraPosInMap = (viewPos / static_cast<float>(blockSizeInPixels)) + middleOfMap;

    int additionalBlocks = 3;
    Vec2 blocksAmount = Window::GetSize() / static_cast<float>(blockSizeInPixels) + static_cast<float>(additionalBlocks);
    
    chunk_t chunk = {
        { cameraPosInMap.x - blocksAmount.x / 2, cameraPosInMap.x + blocksAmount.x / 2 },
        { cameraPosInMap.y - blocksAmount.y / 2, cameraPosInMap.y + blocksAmount.y / 2 }
    };
    
    quadVao->Bind();
    tileMap->Bind();

    BlockType lastType = BlockType::Empty;

    // Rendering only a chunk
    for (int x = chunk.x.start; x < chunk.x.end; x++) {
        for (int y = chunk.y.start; y < chunk.y.end; y++) {
            BlockType type = blocks[x][y];
            if (type == BlockType::Empty) {
                // Check if this block throws light -> check if it's next to a visible block.
                if (blocks[x][y - 1] != BlockType::Empty) {
                    // It does throw light!
                    Vec2 blockPosition = Vec2((x - cameraPosInMap.x) * blockSizeInPixels + viewPos.x, (y - cameraPosInMap.y) * blockSizeInPixels + viewPos.y);
                    lightData.blocksThrowingLight.emplace_back(Block { BlockType::Empty, blockPosition });
                }

                continue;
            }

            Vec2 blockPosition = Vec2((x - cameraPosInMap.x) * blockSizeInPixels + viewPos.x, (y - cameraPosInMap.y) * blockSizeInPixels + viewPos.y);

            // To make sure we don't load the same offset that's currently loaded in the shader.
            if (type != lastType) {
                Vec2 offset = tileDictionary[type] * static_cast<float>(blockSizeInPixels) / tileMap->GetSize();

                shader->SetVec2("u_Offset", Math::ToPtr(offset));
                lastType = type;
            }

            shader->SetVec2("u_Pos", Math::ToPtr(blockPosition));
            
            glDrawElements(GL_TRIANGLES, quadVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
        }
    }

    tileMap->Unbind();
    quadVao->Unbind();

    return lightData;
}

void GenerateMap(Map::blocks_t& map, Size size) {
    const int HORIZONT_LINE = 500;

    int width = static_cast<int>(size.x);
    int height = static_cast<int>(size.y);

    map.resize(width);
    for (int i = 0; i < width; i++) {
        map[i].resize(height);
        for (int j = 0; j < height; j++) {
            map[i][j] = BlockType::Empty;
        }
    }

    for (int x = 0; x < width; x++) {
        map[x][HORIZONT_LINE] = BlockType::Grass;
    }

    for (int y = 0; y < HORIZONT_LINE; y++) {
        for (int x = 0; x < width; x++) {
            map[x][y] = BlockType::Dirt;
        }
    }

    for (int i = 0; i < 9; i++) {
        map[1000 + i][500] = BlockType::Empty;
        map[1000 + i][499] = BlockType::Empty;
        map[1000 + i][498] = BlockType::Empty;
        map[1000 + i][497] = BlockType::Empty;
    }
}