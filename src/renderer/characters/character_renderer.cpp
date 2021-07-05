#include "character_renderer.h"

#include "assets/image_asset.h"
#include "assets/text_asset.h"

#include "maths/primitive.h"

#include "core/window.h"

#include "werwel/werwel.h"

CharacterRenderer::CharacterRenderer(const std::vector<Ref<Character>> &characters, const Ref<Camera> &camera) : characters{characters}, camera{camera}
{
	const ww::ImageAsset characterBodyTextureAsset("assets/images/characters/humanoid_body.png", NF_ROOT);
	characterBodyTexture = CreateRef<ww::Texture>(
		characterBodyTextureAsset.GetSize(),
		characterBodyTextureAsset.GetData(),
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		ww::Texture::Parameters_t{
			ww::Texture::SetInterpolation(ww::Interpolation::Constant)});

	const ww::ImageAsset characterHandTextureAsset("assets/images/characters/humanoid_hand.png", NF_ROOT);
	characterHandTexture = CreateRef<ww::Texture>(
		characterHandTextureAsset.GetSize(),
		characterHandTextureAsset.GetData(),
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		ww::Texture::Parameters_t{
			ww::Texture::SetInterpolation(ww::Interpolation::Constant)});

	const auto &vertices = Primitives::Char::Vertices(HUMANOID_SIZE.x, HUMANOID_SIZE.y);
	const auto &indices = Primitives::Char::indices;

	characterVAO = CreateRef<ww::VAO>();
	characterVAO->Bind();
	characterVAO->AddVBO(ww::VBO::Type::Array, ww::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
	characterVAO->AddVBO(ww::VBO::Type::Indices, ww::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);

	ww::TextAsset vsCode("assets/shaders/characters/default.vs", NF_ROOT);
	ww::TextAsset fsCode("assets/shaders/characters/default.fs", NF_ROOT);
	characterShader = CreateRef<ww::Shader>(
		vsCode.GetContent(), fsCode.GetContent(), 
		ww::Uniforms {
			"u_Proj", "u_View", "u_Model",
			"u_Frame", "u_AmountOfFrames",
			"u_Direction", "u_Weapon"
		}
	);
}

void CharacterRenderer::Render()
{
	characterShader->Bind();
	characterShader->SetMat4x4("u_Proj", Math::ToPtr(ww::Window::GetSpace()));
	characterShader->SetMat4x4("u_View", Math::ToPtr(camera->GetTransform()));

	characterVAO->Bind();
	characterVAO->GetIndexBuffer()->Bind();

	for (const auto &character : characters)
	{
		Mat4 characterTransform = character->GetTransform();
		characterTransform = Math::Translate(characterTransform, Vec3(character->animator->GetDirection() * -24.0f, 0, 0));
		characterTransform = Math::Scale(characterTransform, Vec3(character->animator->GetDirection(), 1, 1));
		Mat4 bodyTransform = characterTransform;

		characterShader->SetMat4x4("u_Model", Math::ToPtr(bodyTransform));
		characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(character->animator->walkingAnimation->GetTime()), 0.0f)));
		characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(Vec2(14.0f, 1.0f)));
		characterShader->SetFloat("u_Direction", static_cast<float>(character->animator->GetDirection()));
		characterShader->SetFloat("u_Weapon", 0.0f);

		characterBodyTexture->Bind();
		ww::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());

		Ref<Animation::Clip> &animation = character->animator->GetCurrentAnimation();

		if (character->player->GetCurrentItem().first)
		{
			const Icon &icon = character->player->GetCurrentItem();
			animation->ApplyTo(characterTransform);
			characterTransform = Math::Scale(characterTransform, Vec3(icon.first->GetTileSize() / HUMANOID_SIZE * 2.0f, 1));

			characterShader->SetMat4x4("u_Model", Math::ToPtr(characterTransform));
			characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(animation->GetTime()), character->animator->GetState())));
			characterShader->SetFloat("u_Weapon", 1.0f);
			characterShader->SetVec2("u_Frame", Math::ToPtr(icon.second));
			characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(icon.first->GetAmountOfTiles()));

			icon.first->Bind();
			ww::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());
		}

		characterShader->SetMat4x4("u_Model", Math::ToPtr(bodyTransform));
		characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(animation->GetTime()), character->animator->GetState())));
		characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(Vec2(14.0f, 2.0f)));
		characterShader->SetFloat("u_Weapon", 0.0f);
		characterHandTexture->Bind();
		ww::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());
	}
}