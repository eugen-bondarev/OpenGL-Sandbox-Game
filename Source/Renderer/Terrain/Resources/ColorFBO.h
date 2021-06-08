#pragma once

#include "Werwel/FBO.h"

class ColorFBO : public Werwel::FBO {
public:
	inline ColorFBO(Size size) : Werwel::FBO(Werwel::Size { size.x, size.y }, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 }) {
		BeginInit();

		attachments[GL_COLOR_ATTACHMENT0] = CreateRef<Werwel::Texture>(
			Werwel::Size { size.x, size.y },
			nullptr,
			GL_RGBA,
			GL_RGBA,
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

	inline void BindTexture(GLuint slot) const {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
	}

	inline void UnbindTexture(GLuint slot) const {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};