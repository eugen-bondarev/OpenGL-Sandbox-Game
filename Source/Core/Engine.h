#pragma once

#include "Gpu/Shader.h"
#include "Gpu/ColorFbo.h"
#include "Gpu/LightFbo.h"

#include "Renderer/Entities/Sprite.h"

#include "Game/Map.h"

#include <memory>

class Engine {
public:
	Engine();
	void InitResources();
	bool IsRunning() const;
	void BeginFrame();
	void Control();
	void Render();
	void EndFrame();
	~Engine();

private:
	std::shared_ptr<Map> map;

	// std::shared_ptr<Vao> tileVao;
	// std::shared_ptr<Texture> tileMap;
	// std::shared_ptr<Shader> shader;

	// Mat4 viewMatrix{Mat4(1)};
	// Vec2 viewPos{Vec2(0)};

	// Mat4 projMatrix{Mat4(1)};

	float currentTime{0};
	float delta{0};
	float lastTime{0};

	float fpsTimer{0};
	float fps{0};
};