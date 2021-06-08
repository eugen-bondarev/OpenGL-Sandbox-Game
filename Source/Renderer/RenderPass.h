#pragma once

#include "Werwel/FBO.h"
#include "Werwel/Shader.h"

template <typename T>
class RenderPass {
public:
	RenderPass() = default;

	inline Ref<T>& GetFbo() {
		return fbo;
	}

	inline Ref<Werwel::Shader>& GetShader() {
		return shader;
	}

protected:
	Ref<T> fbo;
	Ref<Werwel::Shader> shader;
};