#pragma once

#include "Assets/ImageAsset.h"

#include "Werwel/VAO.h"
#include "Werwel/Texture.h"

enum class Interpolation {
    Linear = GL_LINEAR,
    Constant = GL_NEAREST
};

class Sprite {
public:
    Sprite(ImageAsset& asset, Interpolation interpolation = Interpolation::Constant, bool alphaChannel = true);

    template <typename... Args>
    void SetPosition(Args&&... args) {
        position = Vec2(std::forward<Args>(args)...);
        CalculateTransformation();
    }

    template <typename... Args>
    void SetScale(Args&&... args) {
        scale = Vec2(std::forward<Args>(args)...);
        CalculateTransformation();
    }

    inline Mat4 GetTransformation() const {
        return transformation;
    }

    inline std::shared_ptr<Werwel::Texture>& GetTexture() {
        return texture;
    }

private:
    Vec2 position { Vec2(0) };
    Vec2 scale { Vec2(1) };
    
    Vec2 pixelSize { 64, 64 };

    Mat4 transformation;
    void CalculateTransformation();

    std::shared_ptr<Werwel::Texture> texture;
};