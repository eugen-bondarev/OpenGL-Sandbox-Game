#pragma once

#include "Gpu/Vao.h"
#include "Gpu/Shader.h"

#include "ColorPass.h"
#include "LightPass.h"

class CompositionPass {
public:
    CompositionPass();

    void Execute(std::shared_ptr<ColorPass>& colorPass, std::shared_ptr<LightPass>& lightPass);

private:
    std::unique_ptr<Vao> canvas;
    std::unique_ptr<Shader> shader;
};