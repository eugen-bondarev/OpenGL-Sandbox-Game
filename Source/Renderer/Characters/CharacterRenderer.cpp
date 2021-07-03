#include "CharacterRenderer.h"

#include "Assets/ImageAsset.h"
#include "Assets/TextAsset.h"

#include "Maths/Primitive.h"

#include "Core/Window.h"

CharacterRenderer::CharacterRenderer(const std::vector<Ref<Character>>& characters, const Ref<Camera>& camera) : characters { characters }, camera { camera } {
	const ImageAsset characterBodyTextureAsset("Assets/Images/Characters/Char_NoHand2.png");
	characterBodyTexture = CreateRef<Werwel::Texture>(
		characterBodyTextureAsset.GetSize(),
		characterBodyTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const ImageAsset characterHandTextureAsset("Assets/Images/Characters/Hand2.png");
	characterHandTexture = CreateRef<Werwel::Texture>(
		characterHandTextureAsset.GetSize(),
		characterHandTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const ImageAsset pickaxeTextureAsset("Assets/Images/Pickaxe1.png");
	pickaxeTexture = CreateRef<Werwel::Texture>(
		pickaxeTextureAsset.GetSize(),
		pickaxeTextureAsset.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
	);

	const auto& vertices = Primitives::Char::Vertices(32 * 2.0f, 48 * 2.0f);
	const auto& indices = Primitives::Char::indices;

	characterVAO = CreateRef<Werwel::VAO>();
	characterVAO->Bind();		
		characterVAO->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
		characterVAO->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);

	TextAsset vsCode("Assets/Shaders/Characters/Default.vs");
	TextAsset fsCode("Assets/Shaders/Characters/Default.fs");
	characterShader = CreateRef<Werwel::Shader>(
		vsCode.GetContent(), fsCode.GetContent(),
		"u_Proj", "u_View", "u_Model", "u_Frame", "u_Frame1", "u_Frames_Vert", "u_Direction", "u_Weapon"
	);

	pickaxe = TextureAtlas::Get<ToolsTileMap>(TextureAtlasType::Tools);
}

void CharacterRenderer::Render() {  
	characterShader->Bind();
	characterShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
	characterShader->SetMat4x4("u_View", Math::ToPtr(camera->GetTransform()));

  characterVAO->Bind();
  characterVAO->GetIndexBuffer()->Bind();

  for (const auto& character : characters) {
		Mat4 t = character->GetTransform();
		t = Math::Translate(t, Vec3(character->animator->GetDirection() * -24.0f, 0, 0));
		t = Math::Scale(t, Vec3(character->animator->GetDirection(), 1, 1));
    characterShader->SetMat4x4("u_Model", Math::ToPtr(t));

		characterShader->SetFloat("u_Frame", truncf(character->animator->GetFrame()));
		characterShader->SetFloat("u_Frame1", 0);
		characterShader->SetFloat("u_Frames_Vert", 1.0f);
		characterShader->SetFloat("u_Direction", static_cast<float>(character->animator->GetDirection()));
		characterShader->SetFloat("u_Weapon", 0.0f);
  		characterBodyTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		if (character->animator->state == 1) {
			characterShader->SetFloat("u_Frame", truncf(character->animator->GetAttackFrame()));
		} else {
			characterShader->SetFloat("u_Frame", truncf(character->animator->GetFrame()));
		}
		
		characterShader->SetFloat("u_Frame1", character->animator->state);
		characterShader->SetFloat("u_Frames_Vert", 2.0f);
		
		int anim = truncf( character->animator->state == 1 ? character->animator->GetAttackFrame() : character->animator->GetFrame() );
		int dir = character->animator->GetDirection();

		t = character->GetTransform();
		t = Math::Translate(t, Vec3(2 + 5 * dir, 24, 0));

		Vec2 v = { 0, 0 };
		if (character->animator->state == 1) {
			t = Math::Rotate(t, Math::Radians(24.0f * dir), Vec3(0, 0, 1));
			t = Math::Rotate(t, Math::Radians(character->animator->GetAttackFrame() * -10 * dir), Vec3(0, 0, 1));			
			if (anim == 0) {
				v = Vec2(16, 28);
			} else if (anim == 1) {
				v = Vec2(16, 28);
			} else if (anim == 2) {
				v = Vec2(16, 28);
			} else if (anim == 3) {
				v = Vec2(14, 28);
			} else if (anim == 4) {
				v = Vec2(12, 28);
			} else if (anim == 5) {
				v = Vec2(10, 24);
			} else if (anim == 6) {
				v = Vec2(8, 22);
			} else if (anim == 7) {
				v = Vec2(6, 20);
			}
		} else {
			if (anim % 14 == 0) { 				v = Vec2(2, 18);
			} else if (anim % 14 == 1) { v = Vec2(2, 18);
			} else if (anim % 14 == 2) { v = Vec2(4, 18);
			} else if (anim % 14 == 3) { v = Vec2(4, 16);
			} else if (anim % 14 == 4) { v = Vec2(4, 16);
			} else if (anim % 14 == 5) { v = Vec2(2, 16);
			} else if (anim % 14 == 6) { v = Vec2(2, 16);
			} else if (anim % 14 == 7) { v = Vec2(2, 18);
			} else if (anim % 14 == 8) { v = Vec2(0, 18);
			} else if (anim % 14 == 9) { v = Vec2(0, 18);
			} else if (anim % 14 == 10) { v = Vec2(0, 16);
			} else if (anim % 14 == 11) { v = Vec2(0, 16);
			} else if (anim % 14 == 12) { v = Vec2(2, 16);
			} else if (anim % 14 == 13) { v = Vec2(2, 16);
			}
		}
		if (dir == -1) {
			v *= Vec2(dir, 1);
			v += Vec2(32, 0);
		}

		t = Math::Translate(t, Vec3(v, 0));
		t = Math::Translate(t, -Vec3(5, 24, 0));
		t = Math::Scale(t, Vec3(32.0f / Vec2(32 * 2.0f * character->animator->GetDirection(), 48 * 2.0f), 1));


    characterShader->SetMat4x4("u_Model", Math::ToPtr(t));
		characterShader->SetFloat("u_Weapon", 1.0f);
  		// pickaxeTexture->Bind();
			pickaxe->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		t = character->GetTransform();
		t = Math::Translate(t, Vec3(character->animator->GetDirection() * -24.0f, 0, 0));
		t = Math::Scale(t, Vec3(character->animator->GetDirection(), 1, 1));
    characterShader->SetMat4x4("u_Model", Math::ToPtr(t));
		characterShader->SetFloat("u_Frame1", character->animator->state);
		characterShader->SetFloat("u_Frames_Vert", 2.0f);
		characterShader->SetFloat("u_Weapon", 0.0f);
  		characterHandTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}