#pragma once

#include "GPU/FBO.h"
#include "GPU/Shader.h"

template <typename T>
class RenderPass {
public:
    RenderPass() = default;

    inline Ref<T>& GetFbo() {
        return fbo;
    }

    inline Ref<Shader>& GetShader() {
        return shader;
    }

protected:
    Ref<T> fbo;
    Ref<Shader> shader;
};