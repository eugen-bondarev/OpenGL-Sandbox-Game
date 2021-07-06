#pragma once

#include "mgcwel/mgcwel.h"

#include "game/entities/character.h"
#include "game/entities/camera.h"

#include "../renderer.h"

class CharacterRenderer : public IRenderer
{
public:
	CharacterRenderer(const std::vector<Ref<Character>> &characters, const Ref<Camera> &camera);
	void Render() override;

	inline static const Vec2 HUMANOID_SIZE = Vec2(64.0f, 96.0f);

private:
	const std::vector<Ref<Character>> &characters;
	const Ref<Camera> &camera;

	Ref<mw::Texture> characterBodyTexture;
	Ref<mw::Texture> characterHandTexture;

	Ref<mw::VAO> characterVAO;
	Ref<mw::Shader> characterShader;

	CharacterRenderer(const CharacterRenderer &) = delete;
	CharacterRenderer &operator=(const CharacterRenderer &) = delete;
};