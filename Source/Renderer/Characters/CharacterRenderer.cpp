// #include "CharacterRenderer.h"

// #include "Assets/ImageAsset.h"
// #include "Assets/TextAsset.h"

// #include "Math/Primitive.h"

// #include "Core/Window.h"

// CharacterRenderer::CharacterRenderer() {
// 	const ImageAsset characterTextureAsset("Assets/Images/Characters/Character.png");
// 	characterTexture = CreateRef<Werwel::Texture>(
// 		Werwel::Size(characterTextureAsset.GetSize().x, characterTextureAsset.GetSize().y),
// 		characterTextureAsset.GetData(),
// 		GL_RGBA,
// 		GL_RGBA,
// 		GL_UNSIGNED_BYTE,
// 		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
// 		Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
// 	);
// 	const auto& vertices = Primitives::Char::Vertices(16 * 3, 16 * 4);
// 	const auto& indices = Primitives::Char::indices;

// 	characterVAO = CreateRef<Werwel::VAO>();
// 	characterVAO->Bind();		
// 		characterVAO->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
// 		characterVAO->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);

// 	TextAsset vsCode("Assets/Shaders/Characters/Default.vs");
// 	TextAsset fsCode("Assets/Shaders/Characters/Default.fs");
// 	characterShader = CreateRef<Werwel::Shader>(
// 		vsCode.GetContent(), fsCode.GetContent(),
// 		"u_Proj", "u_View", "u_Model"
// 	);
// }

// void CharacterRenderer::Render(const std::vector<Ref<Character>>& characters, Ref<Camera>& camera) {  
// 	characterShader->Bind();
// 	characterShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
// 	characterShader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));

//   characterVAO->Bind();
//   characterVAO->GetIndexBuffer()->Bind();
//   characterTexture->Bind();

//   for (const auto& character : characters) {
//     characterShader->SetMat4x4("u_Model", Math::ToPtr(character->GetModelMatrix()));
// 		glDrawElements(GL_TRIANGLES, characterVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
//   }
// }