#pragma once

#include "game/world/woods/woods.h"
#include "game/entities/camera.h"
#include "renderer/renderer.h"

#include "mgcwel/mgcwel.h"

class WoodsRenderer : public IRenderer
{
public:
	WoodsRenderer(const Ref<Woods> &woods, const Ref<Camera> &camera);

	void Render() override;

	std::vector<Vec2> visibleTrees;

private:
	const Ref<Woods> &woods;
	const Ref<Camera> &camera;

	Vec2 barkSize{16, 204};
	Vec2 leavesSize{128, 128};

	Mat4 barkModelMatrix{1};
	Mat4 leavesModelMatrix{1};

	struct
	{
		Ref<mw::Texture> barkTexture;
		Ref<mw::Texture> leavesTexture;

		Ref<mw::Shader> shader;

		Ref<mw::VAO> barkVAO;
		Ref<mw::VBO> vbo;
	} pipeline;

	WoodsRenderer(const WoodsRenderer &) = delete;
	WoodsRenderer &operator=(const WoodsRenderer &) = delete;
};