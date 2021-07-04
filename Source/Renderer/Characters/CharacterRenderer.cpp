#include "CharacterRenderer.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Maths/Primitive.h"

#include "Core/Window.h"

#include "Werwel/GraphicsContext.h"

CharacterRenderer::CharacterRenderer(const std::vector<Ref<Character>>& characters, const Ref<Camera>& camera) : characters { characters }, camera { camera } {
	const ImageAsset characterBodyTextureAsset("Assets/Images/Characters/Humanoid_Body.png");
	characterBodyTexture = CreateRef<Werwel::Texture>(
		characterBodyTextureAsset.GetSize(),
		characterBodyTextureAsset.GetData(),
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::Parameters_t {
			Werwel::Texture::SetInterpolation(Werwel::Interpolation::Constant)
		}
	);

	const ImageAsset characterHandTextureAsset("Assets/Images/Characters/Humanoid_Hand.png");
	characterHandTexture = CreateRef<Werwel::Texture>(
		characterHandTextureAsset.GetSize(),
		characterHandTextureAsset.GetData(),
		GL_RGBA, GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::Parameters_t {
			Werwel::Texture::SetInterpolation(Werwel::Interpolation::Constant)
		}
	);

	const auto& vertices = Primitives::Char::Vertices(HUMANOID_SIZE.x, HUMANOID_SIZE.y);
	const auto& indices = Primitives::Char::indices;

	characterVAO = CreateRef<Werwel::VAO>();
	characterVAO->Bind();		
		characterVAO->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		characterVAO->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);

	TextAsset vsCode("Assets/Shaders/Characters/Default.vs");
	TextAsset fsCode("Assets/Shaders/Characters/Default.fs");
	characterShader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_Proj", "u_View", "u_Model", 
		"u_Frame", "u_AmountOfFrames", 
		"u_Direction", "u_Weapon"
	);
}

void CharacterRenderer::Render() {  
	characterShader->Bind();
	characterShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	characterShader->SetMat4x4("u_View", Math::ToPtr(camera->GetTransform()));

  characterVAO->Bind();
  characterVAO->GetIndexBuffer()->Bind();

  for (const auto& character : characters) {
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
		Werwel::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());

		Ref<Animation::Clip>& animation = character->animator->GetCurrentAnimation();

		if (character->player->GetCurrentItem().first) {
			const Icon& icon = character->player->GetCurrentItem();
			animation->ApplyTo(characterTransform);
			characterTransform = Math::Scale(characterTransform, Vec3(icon.first->GetTileSize() / HUMANOID_SIZE * 2.0f, 1));

			characterShader->SetMat4x4("u_Model", Math::ToPtr(characterTransform));
			characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(animation->GetTime()), character->animator->GetState())));
			characterShader->SetFloat("u_Weapon", 1.0f);
			characterShader->SetVec2("u_Frame", Math::ToPtr(icon.second));
			characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(icon.first->GetAmountOfTiles()));

			icon.first->Bind();
			Werwel::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());
		}

    characterShader->SetMat4x4("u_Model", Math::ToPtr(bodyTransform));
		characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(animation->GetTime()), character->animator->GetState())));
		characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(Vec2(14.0f, 2.0f)));
		characterShader->SetFloat("u_Weapon", 0.0f);
		characterHandTexture->Bind();
		Werwel::GraphicsContext::DrawIndexed(characterVAO->GetIndexBuffer()->GetIndexCount());
  }
}