#pragma once

#include "Gpu/Framebuffer.h"
#include "Gpu/Shader.h"

template <typename T>
class RenderPass {
public:
    RenderPass() = default;

    inline std::shared_ptr<T>& GetFbo() {
        return fbo;
    }

    inline std::shared_ptr<Shader>& GetShader() {
        return shader;
    }

protected:
    std::shared_ptr<T> fbo;
    std::shared_ptr<Shader> shader;
};