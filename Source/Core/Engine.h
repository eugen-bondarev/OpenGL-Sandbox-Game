#pragma once

#include "Game/Map.h"

#include "Gpu/Shader.h"
#include "Gpu/ColorFbo.h"
#include "Gpu/LightFbo.h"

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

    struct {
        std::unique_ptr<LightFbo> fbo;
        std::unique_ptr<Shader> shader;
    } lightPass;

    struct {
        std::unique_ptr<Vao> canvas;
        std::unique_ptr<Shader> shader;
    } composition;

    std::unique_ptr<Map> map;
    
    // Light pass
    std::unique_ptr<Vao> lightVao;
    std::unique_ptr<Texture> lightTexture;
    // Light pass

    Mat4 viewMatrix { Mat4(1) };
    Vec2 viewPos { Vec2(0) };
};