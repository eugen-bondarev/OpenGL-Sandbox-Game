#include "Map.h"

#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"
#include "Core/Window.h"

Map::Map(Size size, int blockSizeInPixels) {
    this->size = size;
    this->blockSizeInPixels = blockSizeInPixels;

    ImageAsset image("Assets/Images/Map2.png");
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

    for (int x = 0; x < size.x; x++) {
        for (int y = 0; y < size.y; y++) {
            if (blocks[x][y].type == BlockType::Empty) {
                CalculateEmptyNeighbours(x, y);
            }
        }
    }
}

void Map::CalculateEmptyNeighbours(int x, int y) {
    if (x == 0 || y == 0 || x == size.x - 1 || y == size.y - 1) return;

    if (blocks[x][y - 1].type != BlockType::Empty) {
        blocks[x][y - 1].mt = true;
    }

    if (blocks[x][y + 1].type != BlockType::Empty) {
        blocks[x][y + 1].mb = true;
    }

    if (blocks[x - 1][y].type != BlockType::Empty) {
        blocks[x - 1][y].rm = true;
    }

    if (blocks[x + 1][y].type != BlockType::Empty) {
        blocks[x + 1][y].lm = true;
    }
}

Vec2 Map::PickTile(int x, int y) {
    if (blocks[x][y].mt) {
        if (blocks[x][y].rm) {
            return { 1, -1 };
        }

        if (blocks[x][y].lm) {
            return { -1, -1 };
        }

        if (!blocks[x][y].lm && !blocks[x][y].rm) {
            return { 0, -1 };
        }
    } else if (blocks[x][y].mb) {
        if (blocks[x][y].rm) {
            return { 1, 1 };
        }

        if (blocks[x][y].lm) {
            return { -1, 1 };
        }

        if (!blocks[x][y].lm && !blocks[x][y].rm) {
            return { 0, 1 };
        }
    } else {
        if (blocks[x][y].rm) {
            return { 1, 0 };
        }

        if (blocks[x][y].lm) {
            return { -1, 0 };
        }
    }

    return Vec2 { 0 };
}

LightData Map::Render(std::shared_ptr<Shader>& shader, Vec2 viewPos) {

    Vec2 cameraPosInMap = (viewPos / static_cast<float>(blockSizeInPixels)) + middleOfMap;

    int additionalBlocks = 3;
    Vec2 blocksAmount = Window::GetSize() / static_cast<float>(blockSizeInPixels) + static_cast<float>(additionalBlocks);
    
    chunk = {
        { cameraPosInMap.x - blocksAmount.x / 2, cameraPosInMap.x + blocksAmount.x / 2 },
        { cameraPosInMap.y - blocksAmount.y / 2, cameraPosInMap.y + blocksAmount.y / 2 }
    };
    
    quadVao->Bind();
    tileMap->Bind();

    if (recalculateLight) {
        lightData.blocksThrowingLight.clear();
    }

    debug.blocksRendered = 0;

    for (int x = chunk.x.start; x < chunk.x.end; x++) {
        for (int y = chunk.y.start; y < chunk.y.end; y++) {
            BlockType type = blocks[x][y].type;
            if (type == BlockType::Empty) {
                if (recalculateLight) {
                    if (blocks[x][y - 1].type != BlockType::Empty || blocks[x][y + 1].type != BlockType::Empty) {
                        Vec2 blockPosition = Vec2((x - cameraPosInMap.x) * blockSizeInPixels + viewPos.x, (y - cameraPosInMap.y) * blockSizeInPixels + viewPos.y);
                        lightData.blocksThrowingLight.emplace_back(Block(BlockType::Empty, blockPosition));
                    }                    
                    CalculateEmptyNeighbours(x, y);
                }
                continue;
            }

            Vec2 offsetTextureSpace = tileDictionary[type];
            Vec2 additionalOffset = PickTile(x, y);
            Vec2 offset = (offsetTextureSpace + additionalOffset) * static_cast<float>(blockSizeInPixels) / tileMap->GetSize();
            Vec2 blockPosition = Vec2((x - cameraPosInMap.x) * blockSizeInPixels + viewPos.x, (y - cameraPosInMap.y) * blockSizeInPixels + viewPos.y);

            shader->SetVec2("u_Offset", Math::ToPtr(offset));
            shader->SetVec2("u_Pos", Math::ToPtr(blockPosition));
            
            // glDrawElements(GL_TRIANGLES, quadVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            debug.blocksRendered += 1;
        }
    }

    recalculateLight = false;

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

    for (int i = 0; i < 9; i++) {
        map[1000 + i][490] = BlockType::Empty;
        map[1000 + i][489] = BlockType::Empty;
        map[1000 + i][488] = BlockType::Empty;
        map[1000 + i][487] = BlockType::Empty;
    }
}