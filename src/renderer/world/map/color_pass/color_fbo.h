#pragma once

#include "werwel/werwel.h"

class ColorFBO : public ww::FBO
{
public:
	ColorFBO(Vec2 size);

	void BindTexture(GLuint slot) const;
	void UnbindTexture(GLuint slot) const;

	GLuint GetTextureHandle() const;
	const Ref<ww::Texture> &GetTexture() const;
};