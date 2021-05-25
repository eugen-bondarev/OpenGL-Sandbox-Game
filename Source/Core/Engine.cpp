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

    map = std::make_shared<Map>(Size { 2000, 1000 }, 16);

    colorPass = std::make_shared<ColorPass>();
    lightPass = std::make_shared<LightPass>();
    compositionPass = std::make_unique<CompositionPass>();
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
    LightData lightData;

    colorPass->Execute(lightData, viewMatrix, viewPos, map);
    lightPass->Execute(lightData, viewMatrix);
    compositionPass->Execute(colorPass, lightPass);
}

void Engine::EndFrame() {
    Gui::End();
    Window::SwapBuffers();
}

Engine::~Engine() {
    Gui::Destroy();
    Window::Destroy();
}