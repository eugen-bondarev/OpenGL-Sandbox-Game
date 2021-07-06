#include "character_renderer.h"

#include "maths/primitive.h"

#include "mgcwel/mgcwel.h"

CharacterRenderer::CharacterRenderer(const std::vector<Ref<Character>> &characters, const Ref<Camera> &camera) : characters{characters}, camera{camera}
{
	const mw::ImageAsset characterBodyTextureAsset("assets/images/characters/humanoid_body.png");
	characterBodyTexture = CreateRef<mw::Texture>(
		characterBodyTextureAsset.GetSize(),
		characterBodyTextureAsset.GetData(),
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		mw::Texture::Parameters_t{
			mw::Texture::SetInterpolation(mw::Interpolation::Constant)});

	const mw::ImageAsset characterHandTextureAsset("assets/images/characters/humanoid_hand.png");
	characterHandTexture = CreateRef<mw::Texture>(
		characterHandTextureAsset.GetSize(),
		characterHandTextureAsset.GetData(),
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		mw::Texture::Parameters_t{
			mw::Texture::SetInterpolation(mw::Interpolation::Constant)});

	const auto &vertices = Primitives::Char::Vertices(HUMANOID_SIZE.x, HUMANOID_SIZE.y);
	const auto &indices = Primitives::Char::indices;

	characterVAO = CreateRef<mw::VAO>();
	characterVAO->Bind();
	characterVAO->AddVBO(mw::VBO::Type::Array, mw::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
	characterVAO->AddVBO(mw::VBO::Type::Indices, mw::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);

	mw::TextAsset vsCode("assets/shaders/characters/default.vs");
	mw::TextAsset fsCode("assets/shaders/characters/default.fs");
	characterShader = CreateRef<mw::Shader>(
		vsCode.GetContent(), fsCode.GetContent(), 
		mw::Uniforms {
			"u_Proj", "u_View", "u_Model",
			"u_Frame", "u_AmountOfFrames",
			"u_Direction", "u_Weapon"
		}
	);
}

void CharacterRenderer::Render()
{
	characterShader->Bind();
	characterShader->SetMat4x4("u_Proj", Math::ToPtr(mw::Window::GetSpace()));
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
		mw::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());

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
			mw::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());
		}

		characterShader->SetMat4x4("u_Model", Math::ToPtr(bodyTransform));
		characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(animation->GetTime()), character->animator->GetState())));
		characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(Vec2(14.0f, 2.0f)));
		characterShader->SetFloat("u_Weapon", 0.0f);
		characterHandTexture->Bind();
		mw::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());
	}
}