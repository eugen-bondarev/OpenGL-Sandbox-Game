#include "CharacterRenderer.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Maths/Primitive.h"

#include "Core/Window.h"

CharacterRenderer::CharacterRenderer(const std::vector<Ref<Character>>& characters, const Ref<Camera>& camera) : characters { characters }, camera { camera } {
	const ImageAsset characterBodyTextureAsset("Assets/Images/Characters/Humanoid_Body.png");
	characterBodyTexture = CreateRef<Werwel::Texture>(
		characterBodyTextureAsset.GetSize(),
		characterBodyTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const ImageAsset characterHandTextureAsset("Assets/Images/Characters/Humanoid_Hand.png");
	characterHandTexture = CreateRef<Werwel::Texture>(
		characterHandTextureAsset.GetSize(),
		characterHandTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
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
	
	animation0.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 8, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 8, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 8, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);

	animation1.keyFrames.emplace_back(Vec2 { 24, 36 },  30);
	animation1.keyFrames.emplace_back(Vec2 { 26, 34 },  15);
	animation1.keyFrames.emplace_back(Vec2 { 28, 32 },  0);
	animation1.keyFrames.emplace_back(Vec2 { 30, 30 }, -25);
	animation1.keyFrames.emplace_back(Vec2 { 28, 28 }, -45);
	animation1.keyFrames.emplace_back(Vec2 { 26, 26 }, -60);
	animation1.keyFrames.emplace_back(Vec2 { 24, 24 }, -75);
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
    characterShader->SetMat4x4("u_Model", Math::ToPtr(characterTransform));

		characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(character->animator->GetFrame()), 0)));
		characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(Vec2(14.0f, 1.0f)));
		characterShader->SetFloat("u_Direction", static_cast<float>(character->animator->GetDirection()));
		characterShader->SetFloat("u_Weapon", 0.0f);
  		characterBodyTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);


		if (character->player->GetCurrentItem().first) {
			const Icon& icon = character->player->GetCurrentItem();

			float selectedAnimation = character->animator->state == 1 ? character->animator->GetAttackFrame() : character->animator->GetFrame();
			characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(selectedAnimation), character->animator->state)));

			Animation& animation = character->animator->state == 1 ? animation1 : animation0;

			int anim = static_cast<int>(truncf(character->animator->state == 1 ? character->animator->GetAttackFrame() : character->animator->GetFrame())) % animation.keyFrames.size();

			animation.keyFrames[anim].ApplyTo(characterTransform);
			
			characterTransform = Math::Scale(characterTransform, Vec3(icon.first->GetTileSize() / HUMANOID_SIZE * 2.0f, 1));

			characterShader->SetMat4x4("u_Model", Math::ToPtr(characterTransform));
			characterShader->SetFloat("u_Weapon", 1.0f);
			characterShader->SetVec2("u_Frame", Math::ToPtr(icon.second));
			characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(icon.first->GetAmountOfTiles()));
			icon.first->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);			
		}

		characterTransform = character->GetTransform();
		characterTransform = Math::Translate(characterTransform, Vec3(character->animator->GetDirection() * -24.0f, 0, 0));
		characterTransform = Math::Scale(characterTransform, Vec3(character->animator->GetDirection(), 1, 1));
    characterShader->SetMat4x4("u_Model", Math::ToPtr(characterTransform));

		float selectedAnimation = character->animator->state == 1 ? character->animator->GetAttackFrame() : character->animator->GetFrame();
		characterShader->SetVec2("u_Frame", Math::ToPtr(Vec2(truncf(selectedAnimation), character->animator->state)));
		characterShader->SetVec2("u_AmountOfFrames", Math::ToPtr(Vec2(14.0f, 2.0f)));
		characterShader->SetFloat("u_Weapon", 0.0f);
  		characterHandTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}