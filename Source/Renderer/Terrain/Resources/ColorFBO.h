#pragma once

#include "GPU/FBO.h"

class ColorFBO : public FBO {
public:
	inline ColorFBO(Size size) : FBO(size, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 }) {
		BeginInit();

		attachments[GL_COLOR_ATTACHMENT0] = std::make_shared<Texture>(
			size,
			nullptr,
			GL_RGBA,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			std::vector<Texture::param_t> {
				{ ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
				{ ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
			}
		);

		EndInit();
	}

	inline void ColorFBO::Resize(Size size) {
		FBO::Resize(size);
	}

	inline GLuint ColorFBO::GetTextureHandle() const {
		return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
	}

	inline void ColorFBO::BindTexture(GLuint slot) const {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
	}

	inline void ColorFBO::UnbindTexture(GLuint slot) const {
		glActiveTexture(slot);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};