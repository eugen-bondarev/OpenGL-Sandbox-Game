#pragma once

#include "Game/Map.h"

#include "Gpu/Shader.h"

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
    std::unique_ptr<Map> map;
    std::unique_ptr<Shader> shader;

    Mat4 viewMatrix { Mat4(1) };
    Vec2 viewPos { Vec2(0) };
};