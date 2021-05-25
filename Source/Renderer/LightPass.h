#pragma once

#include "RenderPass.h"

#include "Gpu/LightFbo.h"

#include "Entities/Sprite.h"

#include "Game/LightData.h"

class LightPass : public RenderPass<LightFbo> {
public:
    LightPass();

    void Execute(const LightData& lightData, const Mat4& viewMatrix);

    inline std::unique_ptr<Sprite>& GetLightSprite() {
        return lightSprite;
    }

private:
    std::unique_ptr<Sprite> lightSprite;
};