#include "Core/Window.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Gpu/Shader.h"
#include "Gpu/Vao.h"
#include "Gpu/Vertex.h"
#include "Gpu/Texture.h"

#include "Math/Primitive.h"
#include "Scene/Sprite.h"

int main() {
    Window window;

    TextAsset vsCode = TextAsset("Assets/Shaders/Default/Default.vs");
    TextAsset fsCode = TextAsset("Assets/Shaders/Default/Default.fs");

    std::vector<std::shared_ptr<Sprite>> sprites = {
        std::make_shared<Sprite>("Assets/Images/Grass.png"),
        std::make_shared<Sprite>("Assets/Images/Tree1.png")
    };

    Sprite& grass = *sprites[0];
    Sprite& tree = *sprites[1];

    grass.SetScale(1.0f);
    tree.SetPosition(100, 0, 0);
    tree.SetScale(2);

    Shader shader(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_Model");
    shader.Bind();
        shader.SetMat4x4("u_Proj", Math::ToPtr(window.GetSpace()));
    shader.Unbind();

    while (!window.ShouldClose()) {
        window.PollEvents();

        shader.Bind();
            for (auto& model : sprites) {
                shader.SetMat4x4("u_Model", Math::ToPtr(model->GetTransform()));
                model->GetTexture()->Bind();
                    model->GetVao()->Bind();
                        glDrawElements(GL_TRIANGLES, model->GetVao()->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
                    model->GetVao()->Unbind();
                model->GetTexture()->Unbind();
            }
        shader.Unbind();

        window.SwapBuffers();
    }

    return 0;
}
