#pragma once

#include "Game/World.h"
#include "Game/Control/Camera.h"

#include "Renderer/Characters/CharacterRenderer.h"

#include "Renderer/DebugRenderer.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Renderer/ColorPass/ColorPass.h"
#include "Renderer/LightPass/LightFBO.h"

#define LINOW_USE_GLM
#include "Linow/Linow.h"

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
	void PopulateBlockData();

	struct {
		Ref<ColorPass> colorPass;

		struct {
			Ref<LightFBO> fbo;
			Ref<Werwel::Shader> shader;
			struct {
				Ref<Werwel::VAO> vao;
				Ref<Werwel::Texture> texture;
				Ref<Werwel::VBO> dynamicVBO;
			} lightMesh;
			std::vector<Vec2> lightPositions;
		} lightPass;

		struct {
			Ref<Werwel::Shader> shader;
			Ref<Werwel::VAO> canvas;
		} compositionPass;
	} pipeline;

	void InitPipeline();
	void InitColorPass();
	void InitLightPass();
	void InitCompositionPass();

	void LightPass();
	void Compose();

	Ref<Map> map;

	bounds_t lastVisibleChunks;
	bounds_t visibleChunks;

	void OnVisibleChunksChange();
	Ref<Camera> camera;

	Engine(const Engine&) = delete;
	Engine& operator=(const Engine&) = delete;
};