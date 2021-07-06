#include "FBO.h"

namespace mw {

FBO::FBO(Vec2 size, GLbitfield buffersToClear, std::vector<GLuint> drawBuffers)
	: size { size }, buffersToClear{ buffersToClear }, drawBuffers{ drawBuffers }
{
}

FBO::~FBO() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &handle);
}

void FBO::BeginInit() {
	glGenFramebuffers(1, &handle);
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void FBO::EndInit() {
	for (auto& attachment : attachments) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment.first, GL_TEXTURE_2D, attachment.second->GetHandle(), 0);
	}

	if (drawBuffers.size() > 0) {
		glDrawBuffers(drawBuffers.size(), drawBuffers.data());
	} else {
		glDrawBuffer(GL_NONE);
	}

	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer is not complete.\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Bind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void FBO::Unbind() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FBO::Clear() const {
	glClear(buffersToClear);
}

void FBO::Resize(Vec2 size) {
	size = size;

	for (auto& attachment : attachments) {
		attachment.second->Bind();
		attachment.second->Resize(size);
		attachment.second->Unbind();
	}
}

Vec2 FBO::GetSize() const {
	return size;
}

}