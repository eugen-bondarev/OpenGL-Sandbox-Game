#pragma once

#include "light_fbo.h"

#include "werwel/werwel.h"

#include "core/window.h"

#include "assets/text_asset.h"
#include "assets/image_asset.h"

#include "maths/primitive.h"

#include "game/entities/camera.h"

class LightPass
{
public:
	LightPass();
	void Perform(const Ref<Camera> &camera, int amountOfBlocks);

	inline const Ref<LightFBO> &GetFBO() const
	{
		return fbo;
	}

	inline const Ref<ww::VBO> &GetVBO() const
	{
		return vbo;
	}

private:
	Ref<LightFBO> fbo;
	Ref<ww::Shader> shader;
	Ref<ww::VBO> vbo;

	struct
	{
		Ref<ww::VAO> vao;
		Ref<ww::Texture> texture;
	} lightMesh;
	std::vector<Vec2> lightPositions;
};