#pragma once

#include "GPU/FBO.h"

class LightFBO : public FBO {
public:
	inline LightFBO(Size size) : FBO(size, GL_COLOR_BUFFER_BIT, { GL_COLOR_ATTACHMENT0 }) {
		BeginInit();

		attachments[GL_COLOR_ATTACHMENT0] = std::make_shared<Texture>(
			size,
			nullptr,
			GL_RGBA /*GL_R8*/,
			GL_RGBA /*GL_RED*/,
			GL_UNSIGNED_BYTE,
			std::vector<Texture::param_t> {
				{ Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
				{ Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST },
			}
		);

		EndInit();
	}

	inline void Resize(Size size) {
		FBO::Resize(size);
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