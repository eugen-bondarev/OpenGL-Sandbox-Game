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
		"u_Proj", "u_View", "u_Model", "u_Frame", "u_Frame1", "u_Frames_Vert", "u_Frames_Hor", "u_Direction", "u_Weapon"
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
		
		int dir = character->animator->GetDirection();
		int state = character->animator->state;

		// t = character->GetTransform();
		
		struct KeyFrame {
			KeyFrame(Vec2 pos, float rot) : pos { pos }, rot { rot } { }
			Vec2 pos  { 0, 0 };
			float rot { 0 };
		};

		std::vector<KeyFrame> animation;

		if (state == 1) {
			animation.emplace_back(Vec2 { 24, 36 }, 15 - 45);
			animation.emplace_back(Vec2 { 26, 34 }, 30 - 45);
			animation.emplace_back(Vec2 { 28, 32 }, 45 - 45);
			animation.emplace_back(Vec2 { 30, 30 }, 70 - 45);
			animation.emplace_back(Vec2 { 28, 28 }, 90 - 45);
			animation.emplace_back(Vec2 { 26, 26 }, 105 - 45);
			animation.emplace_back(Vec2 { 24, 24 }, 120 - 45);
		} else {
			animation.emplace_back(Vec2 { 6, 18 }, 0);
			animation.emplace_back(Vec2 { 6, 18 }, 0);
			animation.emplace_back(Vec2 { 8, 18 }, 0);
			animation.emplace_back(Vec2 { 8, 16 }, 0);
			animation.emplace_back(Vec2 { 8, 16 }, 0);
			animation.emplace_back(Vec2 { 6, 16 }, 0);
			animation.emplace_back(Vec2 { 6, 16 }, 0);
			animation.emplace_back(Vec2 { 6, 18 }, 0);
			animation.emplace_back(Vec2 { 4, 18 }, 0);
			animation.emplace_back(Vec2 { 4, 18 }, 0);
			animation.emplace_back(Vec2 { 4, 16 }, 0);
			animation.emplace_back(Vec2 { 4, 16 }, 0);
			animation.emplace_back(Vec2 { 6, 16 }, 0);
			animation.emplace_back(Vec2 { 6, 16 }, 0);
		}

		int anim = static_cast<int>(truncf(character->animator->state == 1 ? character->animator->GetAttackFrame() : character->animator->GetFrame() )) % animation.size();

		Vec2 setPosition { 0, 0 };
		float setRotation { 0 };

		int clips = animation.size();
		
		if (anim >= 0 && anim < animation.size()) {
			KeyFrame keyFrame = animation[anim];
			setPosition = keyFrame.pos;
			setRotation = keyFrame.rot;
		}
		
		t = Math::Translate(t, Vec3(setPosition, 0.0f));
		t = Math::Rotate(t, Math::Radians(-setRotation), Vec3(0, 0, 1));
		Vec2 size = character->player->GetCurrentItem().first->GetSize() * 2.0f / character->player->GetCurrentItem().first->GetAmountOfTiles();
		t = Math::Scale(t, Vec3(size / Vec2(32 * 2.0f, 48 * 2.0f), 1));

    characterShader->SetMat4x4("u_Model", Math::ToPtr(t));
		characterShader->SetFloat("u_Weapon", 1.0f);
		characterShader->SetFloat("u_Frame", character->player->GetCurrentItem().second.x);
		characterShader->SetFloat("u_Frame1", character->player->GetCurrentItem().second.y);
		characterShader->SetFloat("u_Frames_Hor", character->player->GetCurrentItem().first->GetAmountOfTiles().x);
		characterShader->SetFloat("u_Frames_Vert", character->player->GetCurrentItem().first->GetAmountOfTiles().y);
		
			character->player->GetCurrentItem().first->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);

		t = character->GetTransform();
		t = Math::Translate(t, Vec3(character->animator->GetDirection() * -24.0f, 0, 0));
		t = Math::Scale(t, Vec3(character->animator->GetDirection(), 1, 1));
    characterShader->SetMat4x4("u_Model", Math::ToPtr(t));

		if (character->animator->state == 1) {
			characterShader->SetFloat("u_Frame", truncf(character->animator->GetAttackFrame()));
		} else {
			characterShader->SetFloat("u_Frame", truncf(character->animator->GetFrame()));
		}
		characterShader->SetFloat("u_Frame1", character->animator->state);
		characterShader->SetFloat("u_Frames_Vert", 2.0f);
		characterShader->SetFloat("u_Weapon", 0.0f);
  		characterHandTexture->Bind();
				glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
  }
}