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

    TextAsset vsCode = TextAsset("Assets/Shaders/Default/Default.vs");
    TextAsset fsCode = TextAsset("Assets/Shaders/Default/Default.fs");

    shader = std::make_unique<Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View", "u_Pos", "u_AmountOfTiles", "u_Offset");
    shader->Bind();
        shader->SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 3.0f)));
        shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    shader->Unbind();
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

    shader->Bind();
    shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
        map->Render(*shader, viewPos);
    shader->Unbind();

    ImGui::ShowDemoWindow();
}

void Engine::EndFrame() {
    Gui::End();
    Window::SwapBuffers();
}

Engine::~Engine() {
    Gui::Destroy();
    Window::Destroy();
}