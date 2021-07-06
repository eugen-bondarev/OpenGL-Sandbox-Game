#pragma once

#include "mgcwel/mgcwel.h"

class ColorFBO : public mw::FBO
{
public:
	ColorFBO(Vec2 size);

	void BindTexture(GLuint slot) const;
	void UnbindTexture(GLuint slot) const;

	GLuint GetTextureHandle() const;
	const Ref<mw::Texture> &GetTexture() const;
};