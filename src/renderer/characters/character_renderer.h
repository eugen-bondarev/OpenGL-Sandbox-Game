#pragma once

#include "mgcwel/werwel.h"

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

	Ref<ww::Texture> characterBodyTexture;
	Ref<ww::Texture> characterHandTexture;

	Ref<ww::VAO> characterVAO;
	Ref<ww::Shader> characterShader;

	CharacterRenderer(const CharacterRenderer &) = delete;
	CharacterRenderer &operator=(const CharacterRenderer &) = delete;
};