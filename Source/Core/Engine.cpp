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

	map = std::make_shared<Map>(Size{12, 12}, Size{42, 42}); // ~ 500 x 500

	map->chunks[2][0].Prepare();
	map->chunks[2][0].Rerender();

	map->chunks[3][0].Prepare();
	map->chunks[3][0].Rerender();
}

bool Engine::IsRunning() const {
	return !Window::ShouldClose();
}

void Engine::BeginFrame() {
	Window::Clear();
	Window::PollEvents();
	Gui::Begin();

	currentTime = static_cast<float>(glfwGetTime());
	delta = currentTime - lastTime;
	lastTime = currentTime;
}

void Engine::Control() {
	if (Window::KeyPressed(GLFW_KEY_W)) map->viewPos += Vec2( 0,  1) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_S)) map->viewPos += Vec2( 0, -1) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_A)) map->viewPos += Vec2(-1,  0) * 4.0f;
	if (Window::KeyPressed(GLFW_KEY_D)) map->viewPos += Vec2( 1,  0) * 4.0f;

	map->viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(map->viewPos, 0.0f)));
}

void Engine::Render() {
	ImGui::Begin("Info");
		ImGui::Image((void*)(intptr_t)map->chunks[3][0].GetTexture()->GetHandle(), ImVec2(192, 192), ImVec2(0, 0), ImVec2(1, -1));
	ImGui::End();
}

void Engine::EndFrame() {
	fpsTimer += delta;
	if (fpsTimer >= 1.0f) {
		fps = 1.0f / delta;
		fpsTimer = 0;
	}

	Gui::End();
	Window::SwapBuffers();
}

Engine::~Engine() {
	Gui::Destroy();
	Window::Destroy();
}