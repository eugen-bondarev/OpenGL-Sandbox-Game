#pragma once

#include "Werwel/FBO.h"
#include "Werwel/Texture.h"

#include "Maths/Maths.h"

class LightFBO : public Werwel::FBO {
public:
  inline LightFBO(Vec2 size) : Werwel::FBO(Werwel::Size { size.x, size.y }, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 }) {
    BeginInit();

		attachments[GL_COLOR_ATTACHMENT0] = CreateRef<Werwel::Texture>(
			Werwel::Size { size.x, size.y },
			nullptr,
			GL_R8,
			GL_RED,
			GL_UNSIGNED_BYTE,
			Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
			Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
		);

		EndInit();
  }

	inline void Resize(Werwel::Size size) {
		Werwel::FBO::Resize(size);
	}

	inline GLuint GetTextureHandle() const {
		return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
	}

  inline const Ref<Werwel::Texture>& GetTexture() const {
    return attachments.at(GL_COLOR_ATTACHMENT0);
  }

	inline void BindTexture(GLuint slot) const {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
	}

	inline void UnbindTexture(GLuint slot) const {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};