#pragma once

#include "Werwel/FBO.h"
#include "Werwel/Texture.h"

class ColorFBO : public Werwel::FBO {
public:
  ColorFBO(Vec2 size);

	void BindTexture(GLuint slot) const;
	void UnbindTexture(GLuint slot) const;

	GLuint GetTextureHandle() const;
  const Ref<Werwel::Texture>& GetTexture() const;
};