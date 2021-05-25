#include "Sprite.h"

Sprite::Sprite(ImageAsset& asset, Interpolation interpolation, bool alphaChannel) {    
    texture = std::make_unique<Texture>(
        asset.GetSize(),
        asset.GetData(),
        alphaChannel ? GL_RGBA : GL_RGB,
        alphaChannel ? GL_RGBA : GL_RGB,
        GL_UNSIGNED_BYTE,
        std::vector<Texture::param_t> {
            { ParamType::Int, GL_TEXTURE_MIN_FILTER, static_cast<int>(interpolation) },
            { ParamType::Int, GL_TEXTURE_MAG_FILTER, static_cast<int>(interpolation) }
        }
    );

    CalculateTransformation();
}

void Sprite::CalculateTransformation() {
    transformation = Math::Translate(Mat4(1), Vec3(position, 0.0f));
    transformation = Math::Scale(transformation, Vec3(pixelSize, 1.0f));
    transformation = Math::Scale(transformation, Vec3(scale, 1.0f));
}