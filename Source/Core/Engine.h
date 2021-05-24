#pragma once

#include "Game/Map.h"

#include "Gpu/Shader.h"
#include "Gpu/ColorFbo.h"

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
    struct {
        std::unique_ptr<ColorFbo> fbo;
        std::unique_ptr<Shader> shader;
    } colorPass;

    std::unique_ptr<Map> map;

    Mat4 viewMatrix { Mat4(1) };
    Vec2 viewPos { Vec2(0) };
};