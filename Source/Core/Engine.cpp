#include "Engine.h"

#include "Core/Window.h"
#include "Core/Gui.h"

#include "Assets/TextAsset.h"

Engine::Engine() {
    Window::Create();
    Gui::Create();
}

void Engine::InitResources() {
    map = std::make_unique<Map>(Size { 2000, 1000 }, 16);

    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Default/Default.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Default/Default.fs");
    } shaderCode;

    colorPass.shader = std::make_unique<Shader>(shaderCode.vertex.GetContent(), shaderCode.fragment.GetContent(), "u_Proj", "u_View", "u_Pos", "u_AmountOfTiles", "u_Offset");
    colorPass.shader->Bind();
        colorPass.shader->SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 3.0f)));
        colorPass.shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    colorPass.shader->Unbind();

    colorPass.fbo = std::make_unique<ColorFbo>(Window::GetSize());
}

bool Engine::IsRunning() const {
    return !Window::ShouldClose();
}

void Engine::BeginFrame() {
    Window::Clear();
    Window::PollEvents();
    Gui::Begin();
}

void Engine::Control() {
    if (Window::KeyPressed(GLFW_KEY_W)) viewPos += Vec2( 0,  1) * 4.0f;
    if (Window::KeyPressed(GLFW_KEY_S)) viewPos += Vec2( 0, -1) * 4.0f;
    if (Window::KeyPressed(GLFW_KEY_A)) viewPos += Vec2(-1,  0) * 4.0f;
    if (Window::KeyPressed(GLFW_KEY_D)) viewPos += Vec2( 1,  0) * 4.0f;
}

void Engine::Render() {
    viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

    colorPass.fbo->Bind();
    colorPass.fbo->Clear();
        colorPass.shader->Bind();
        colorPass.shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
            map->Render(*colorPass.shader, viewPos);
        colorPass.shader->Unbind();
    colorPass.fbo->Unbind();

    ImGui::Begin("Color pass");
        ImGui::Image((void*)(intptr_t)colorPass.fbo->GetTextureHandle(), ImVec2(800, 600), ImVec2(0, 0), ImVec2(1, -1));
    ImGui::End();
}

void Engine::EndFrame() {
    Gui::End();
    Window::SwapBuffers();
}

Engine::~Engine() {
    Gui::Destroy();
    Window::Destroy();
}