#pragma once

#include "Werwel/fbo.h"
#include "Werwel/texture.h"

class LightFBO : public Werwel::FBO {
public:
  LightFBO(Vec2 size);

	void BindTexture(GLuint slot) const;
	void UnbindTexture(GLuint slot) const;

	GLuint GetTextureHandle() const;
  const Ref<Werwel::Texture>& GetTexture() const;
};