#pragma once

#include "Game/Map.h"

#include "Gpu/Shader.h"
#include "Gpu/ColorFbo.h"
#include "Gpu/LightFbo.h"

#include "Renderer/ColorPass.h"
#include "Renderer/LightPass.h"
#include "Renderer/CompositionPass.h"

#include "Renderer/Entities/Sprite.h"

#include <memory>

class Engine {
public:
    Engine();
    void InitResources();
    bool IsRunning() const;
    void BeginFrame();
    void Control();
    void Render();
    void EndFrame();
    ~Engine();

private:
    std::shared_ptr<ColorPass> colorPass;
    std::shared_ptr<LightPass> lightPass;
    std::unique_ptr<CompositionPass> compositionPass;

    std::shared_ptr<Map> map;

    Mat4 viewMatrix { Mat4(1) };
    Vec2 viewPos { Vec2(0) };
    Vec2 lastViewPos { Vec2(1) };
    bool redrawMap { true };
};