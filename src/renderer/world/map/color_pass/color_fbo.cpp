#include "color_fbo.h"

ColorFBO::ColorFBO(Vec2 size) : ww::FBO(size, GL_COLOR_BUFFER_BIT, {GL_COLOR_ATTACHMENT0})
{
	BeginInit();

	attachments[GL_COLOR_ATTACHMENT0] = CreateRef<ww::Texture>(
		size,
		nullptr,
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		ww::Texture::Parameters_t{
			ww::Texture::SetInterpolation(ww::Interpolation::Constant)});

	EndInit();
}

GLuint ColorFBO::GetTextureHandle() const
{
	return attachments.at(GL_COLOR_ATTACHMENT0)->GetHandle();
}

const Ref<ww::Texture> &ColorFBO::GetTexture() const
{
	return attachments.at(GL_COLOR_ATTACHMENT0);
}

void ColorFBO::BindTexture(GLuint slot) const
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, GetTextureHandle());
}

void ColorFBO::UnbindTexture(GLuint slot) const
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}