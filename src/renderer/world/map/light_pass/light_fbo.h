#pragma once

#include "werwel/werwel.h"

class LightFBO : public ww::FBO
{
public:
	LightFBO(Vec2 size);

	void BindTexture(GLuint slot) const;
	void UnbindTexture(GLuint slot) const;

	GLuint GetTextureHandle() const;
	const Ref<ww::Texture> &GetTexture() const;
};