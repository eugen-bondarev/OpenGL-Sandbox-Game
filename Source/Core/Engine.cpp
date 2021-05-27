#include "Engine.h"

#include "Core/Window.h"
#include "Core/Gui.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"

#include "Renderer/Entities/RectVao.h"

#include "Gpu/GraphicsContext.h"

Engine::Engine() {
    Window::Create();
    Gui::Create();
}

void Engine::InitResources() {
    Primitives::Rect::Create();

    map = std::make_shared<Map>(Size{ 12, 12 }, Size{ 42, 42 }); // ~ 500 x 500
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

    viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));
}

void Engine::Render() {
    currentTime = static_cast<float>(glfwGetTime());
    delta = currentTime - lastTime;
    lastTime = currentTime;

    ImGui::Begin("Info");

    ImGui::End();

    fpsTimer += delta;
    if (fpsTimer >= 1.0f) {
        fps = 1.0f / delta;
        fpsTimer = 0;
    }
}

void Engine::EndFrame() {
    Gui::End();
    Window::SwapBuffers();
}

Engine::~Engine() {
    Gui::Destroy();
    Window::Destroy();
}