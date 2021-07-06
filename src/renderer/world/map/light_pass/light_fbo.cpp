#include "light_fbo.h"

LightFBO::LightFBO(Vec2 size) : mw::FBO(size, GL_COLOR_BUFFER_BIT, {GL_COLOR_ATTACHMENT0})
{
	BeginInit();

	attachments[GL_COLOR_ATTACHMENT0] = CreateRef<mw::Texture>(
		size,
		nullptr,
		GL_R8,
		GL_RED,
		GL_UNSIGNED_BYTE,
		mw::Texture::Parameters_t{
			mw::Texture::SetInterpolation(mw::Interpolation::Linear)
		}
	);

	EndInit();
}

GLuint LightFBO::GetTextureHandle() const
{
	return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
}

const Ref<mw::Texture> &LightFBO::GetTexture() const
{
	return attachments.at(GL_COLOR_ATTACHMENT0);
}

void LightFBO::BindTexture(GLuint slot) const
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
}

void LightFBO::UnbindTexture(GLuint slot) const
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}