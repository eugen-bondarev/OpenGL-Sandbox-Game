#pragma once

#include "mgcwel/mgcwel.h"

class LightFBO : public mw::FBO
{
public:
	LightFBO(Vec2 size);

	void BindTexture(GLuint slot) const;
	void UnbindTexture(GLuint slot) const;

	GLuint GetTextureHandle() const;
	const Ref<mw::Texture> &GetTexture() const;
};