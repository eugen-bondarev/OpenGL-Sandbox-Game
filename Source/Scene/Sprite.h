#pragma once

#include "Assets/ImageAsset.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

class Sprite {
public:
    inline Sprite(const std::string& texturePath) : transform { Mat4(1) } {
        ImageAsset image(texturePath);
        texture = std::make_shared<Texture>(
            image.GetSize(),
            image.GetData(),
            GL_RGBA,
            image.GetChannels() == 4 ? GL_RGBA : GL_RGB,
            GL_UNSIGNED_BYTE,
            std::vector<Texture::param_t> {
                { ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
                { ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
            }
        );
        vao = std::make_shared<Vao>(Primitives::Quad::vertices, Vertex::GetLayout(), Primitives::Quad::indices);
    }

    template <typename... Args>
    inline void SetPosition(Args&&... args) {
        position = Vec3(std::forward<Args>(args)...);
        RecalculateTransform();
    }

    template <typename... Args>
    inline void SetScale(Args&&... args) {
        scale = Vec3(std::forward<Args>(args)...);
        RecalculateTransform();
    }

    inline void RecalculateTransform() {
        transform = Mat4(1);
        transform = Math::Translate(transform, position);
        transform = Math::Scale(transform, scale);
    }

    inline const Mat4& GetTransform() const {
        return transform;
    }

    inline std::shared_ptr<Texture> & GetTexture() {
        return texture;
    }

    inline std::shared_ptr<Vao> & GetVao() {
        return vao;
    }

private:
    Mat4 transform;
    
    Vec3 position { 0 };
    Vec3 scale { 1 };

    std::shared_ptr<Texture> texture;
    std::shared_ptr<Vao> vao;
};