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
    Window::Create();

    Map map({ 2000, 1000 }, 16);

    TextAsset vsCode = TextAsset("Assets/Shaders/Default/Default.vs");
    TextAsset fsCode = TextAsset("Assets/Shaders/Default/Default.fs");

    Shader shader(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_AmountOfTiles", "u_Offset");
    shader.Bind();
        shader.SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 3.0f)));
        shader.SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    shader.Unbind();

    Mat4 viewMatrix = Mat4(1);
    Vec2 viewPos = Vec2(0);

    while (!Window::ShouldClose()) {
        Window::Clear();
        Window::PollEvents();

        if (Window::KeyPressed(GLFW_KEY_W)) viewPos += Vec2( 0,  1) * 4.0f;
        if (Window::KeyPressed(GLFW_KEY_S)) viewPos += Vec2( 0, -1) * 4.0f;
        if (Window::KeyPressed(GLFW_KEY_A)) viewPos += Vec2(-1,  0) * 4.0f;
        if (Window::KeyPressed(GLFW_KEY_D)) viewPos += Vec2( 1,  0) * 4.0f;

        viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

        shader.Bind();
        shader.SetMat4x4("u_View", Math::ToPtr(viewMatrix));
            map.Render(shader, viewPos);
        shader.Unbind();

        Window::SwapBuffers();
    }

    Window::Destroy();

    return 0;
}
