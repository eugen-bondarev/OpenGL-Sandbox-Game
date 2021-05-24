#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Gpu/Shader.h"
#include "Gpu/Vao.h"
#include "Gpu/Vertex.h"
#include "Gpu/Texture.h"

#include "Math/Primitive.h"
#include "Scene/Sprite.h"

#include "Game/Map.h"

int main() {
    Window window;

    Map map({ 2000, 1000 });

    TextAsset vsCode = TextAsset("Assets/Shaders/Default/Default.vs");
    TextAsset fsCode = TextAsset("Assets/Shaders/Default/Default.fs");

    Shader shader(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_AmountOfTiles", "u_Offset");
    shader.Bind();
        shader.SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 3.0f)));
        shader.SetMat4x4("u_Proj", Math::ToPtr(window.GetSpace()));
    shader.Unbind();

    Mat4 view = Mat4(1);
    Vec2 viewPos = Vec2(0);

    // We need this vector in order to be able to imagine that our camera is in the middle of the map when it's at (0, 0)
    Vec2 middleOfMap = Vec2(map.GetSize().width, map.GetSize().height) / 2.0f;

    std::map<BlockType, Vec2> tileDictionary = {
        { BlockType::Grass, Vec2(1, 0) },
        { BlockType::Dirt, Vec2(4, 1) },
        { BlockType::Stone, Vec2(7, 0) },
    };

    Mat4 model = Mat4(1);

    while (!window.ShouldClose()) {
        window.Clear();
        window.PollEvents();

        if (glfwGetKey(window.GetGlfwWindow(), GLFW_KEY_W)) { viewPos += Vec2( 0,  1) * 4.0f; }
        if (glfwGetKey(window.GetGlfwWindow(), GLFW_KEY_S)) { viewPos += Vec2( 0, -1) * 4.0f; }
        if (glfwGetKey(window.GetGlfwWindow(), GLFW_KEY_A)) { viewPos += Vec2(-1,  0) * 4.0f; }
        if (glfwGetKey(window.GetGlfwWindow(), GLFW_KEY_D)) { viewPos += Vec2( 1,  0) * 4.0f; }

        view = Math::Translate(Mat4(1), Vec3(viewPos.x, viewPos.y, 0));

        Vec2 cameraPosInMap = (viewPos / 16.0f) + middleOfMap;
        int xBlocks = window.GetSize().width / 16.0f + 4.0;
        int yBlocks = window.GetSize().height / 16.0f + 4.0;
        int firstBlockX = cameraPosInMap.x - xBlocks / 2;
        int lastBlockX = firstBlockX + xBlocks;
        int firstBlockY = cameraPosInMap.y - yBlocks / 2;
        int lastBlockY = firstBlockY + yBlocks;

        shader.Bind();
        shader.SetMat4x4("u_View", Math::ToPtr(Math::Inverse(view)));
        map.GetVao()->Bind();
        map.GetTileMap()->Bind();

        // Now that we have our camera position we need to take a chunk of blocks with center in cameraPosInMap.
        BlockType lastType = BlockType::Empty;

        for (int x = firstBlockX; x < lastBlockX; x++) {
            for (int y = firstBlockY; y < lastBlockY; y++) {
                BlockType type = map.blocks[x][y];
                if (type == BlockType::Empty) { continue; }

                Vec2 pos = Vec2((x - cameraPosInMap.x) * 16.0f + viewPos.x, (y - cameraPosInMap.y) * 16.0f + viewPos.y);

                if (type != lastType) {
                    Vec2 offset = tileDictionary[type];
                    offset.x *= 16.0f / 144.0f;
                    offset.y *= 16.0f / 48.0f;

                    shader.SetVec2("u_Offset", Math::ToPtr(offset));
                    lastType = type;
                }

                shader.SetVec2("u_Pos", Math::ToPtr(pos));
                
                glDrawElements(GL_TRIANGLES, map.GetVao()->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            }
        }

        map.GetTileMap()->Unbind();
        map.GetVao()->Unbind();
        shader.Unbind();

        window.SwapBuffers();
    }

    return 0;
}
