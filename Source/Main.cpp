#include "Core/Window.h"
#include "Assets/TextAsset.h"
#include "Gpu/Shader.h"
#include "Gpu/Vao.h"
#include "Gpu/Vertex.h"

int main() {
    Window window;

    TextAsset vsCode = TextAsset("Assets/Shaders/Default/Default.vs");
    TextAsset fsCode = TextAsset("Assets/Shaders/Default/Default.fs");

    Shader shader(vsCode.GetContent(), fsCode.GetContent());
    
    std::vector<Vertex> vertices = {
        { { -0.5, -0.5, 1 }, { 0, 0 }, { 0, 0, 0 } },
        { {  0.5, -0.5, 1 }, { 0, 0 }, { 0, 0, 0 } },
        { {    0,  0.5, 1 }, { 0, 0 }, { 0, 0, 0 } }
    };  

    std::vector<int> indices = {
        0, 1, 2
    };

    Vao vao(vertices, Vertex::GetLayout(), indices);

    while (!window.ShouldClose()) {
        window.PollEvents();

        shader.Bind();
        vao.Bind();
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
        vao.Unbind();
        shader.Unbind();

        window.SwapBuffers();
    }

    return 0;
}
