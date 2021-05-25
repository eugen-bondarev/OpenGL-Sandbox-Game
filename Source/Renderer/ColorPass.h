#pragma once

#include "RenderPass.h"

#include "Gpu/ColorFbo.h"

#include "Game/LightData.h"
#include "Game/Map.h"

class ColorPass : public RenderPass<ColorFbo> {
public:
    ColorPass();

    void Execute(LightData& lightData, const Mat4& viewMatrix, const Vec2& viewPos, std::shared_ptr<Map>& map);
};