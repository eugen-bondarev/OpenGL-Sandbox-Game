#include "Map.h"

#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"

Map::Map(Size size) {
    this->size = size;

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
    
    quadVao = std::make_shared<Vao>(Primitives::Quad::vertices, QuadVertex::GetLayout(), Primitives::Quad::indices);

    GenerateMap(blocks, size);
}

void GenerateMap(Map::blocks_t& map, Size size) {
    const int HORIZONT_LINE = 500;

    int width = static_cast<int>(size.width);
    int height = static_cast<int>(size.height);

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
    }

    for (int i = 490; i > 499; i--)
    for (int i = 0; i < 5; i++) {
        map[1002 + i][499] = BlockType::Empty;
    }
}