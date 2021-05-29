#pragma once

#include "Gpu/Shader.h"
#include "Gpu/ColorFbo.h"
#include "Gpu/LightFbo.h"

#include "Renderer/Entities/Sprite.h"

#include "Game/MapRenderer.h"

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
	static bounds_t GetVisibleChunks(std::shared_ptr<MapRenderer>& map, Pos viewPos);
	std::shared_ptr<Shader> chunkShader;
	std::shared_ptr<Vao> chunkVao;

	std::shared_ptr<MapRenderer> map;

	Vec2 viewPos;
	Mat4 viewMatrix;

	int chunksRendered = 0;

	float currentTime{0};
	float delta{0};
	float lastTime{0};

	float fpsTimer{0};
	float fps{0};
};