#pragma once

#include "Assets/ImageAsset.h"

#include "Math/Math.h"
#include "Math/Primitive.h"

class Sprite {
public:
	inline Sprite(const Str& texturePath) : transform { Mat4(1) } {
		const ImageAsset imageAsset(texturePath);
		texture = CreateRef<Texture>(
			imageAsset.GetSize(),
			imageAsset.GetData(),
			GL_RGBA,
			imageAsset.GetChannels() == 4 ? GL_RGBA : GL_RGB,
			GL_UNSIGNED_BYTE,
			Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
			Texture::param_t { Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
		);
		vao = CreateRef<Vao>(Primitives::Quad::vertices, Vertex::GetLayout(), Primitives::Quad::indices);
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

	inline Ref<Texture> & GetTexture() {
		return texture;
	}

	inline Ref<Vao> & GetVao() {
		return vao;
	}

private:
	Mat4 transform;
	
	Vec3 position { 0 };
	Vec3 scale { 1 };

	Ref<Texture> texture;
	Ref<Vao> vao;
};