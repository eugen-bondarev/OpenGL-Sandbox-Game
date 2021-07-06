#pragma once

#include "light_fbo.h"

#include "mgcwel/mgcwel.h"

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

	inline const Ref<mw::VBO> &GetVBO() const
	{
		return vbo;
	}

private:
	Ref<LightFBO> fbo;
	Ref<mw::Shader> shader;
	Ref<mw::VBO> vbo;

	struct
	{
		Ref<mw::VAO> vao;
		Ref<mw::Texture> texture;
	} lightMesh;
	std::vector<Vec2> lightPositions;
};