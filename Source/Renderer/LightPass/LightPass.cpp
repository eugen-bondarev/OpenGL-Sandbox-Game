#include "LightPass.h"

#include "Core/Window.h"

LightPass::LightPass() {
  fbo = CreateRef<LightFBO>(Window::GetSize());

  TextAsset vsCode("Assets/Shaders/Terrain/LightPassShader.vs");
  TextAsset fsCode("Assets/Shaders/Terrain/LightPassShader.fs");
  shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ProjectionView");
    
  /**
   * Amount of blocks that each light reaches.
   */
  float lightBlocks = 3.5f;

  /**
   * Empirical constant.
   */
  const float lightTextureGradientConstant = 1.35f;
  const auto& vertices = Primitives::Block::Vertices(16 * (lightBlocks * lightTextureGradientConstant) * 2, 16 * (lightBlocks * lightTextureGradientConstant) * 2);
  const auto& indices = Primitives::Block::indices;

  lightMesh.vao = CreateRef<Werwel::VAO>();
  lightMesh.vao->Bind();
    lightMesh.vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
    lightMesh.vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
    vbo = lightMesh.vao->AddVBO(
      Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, 0, sizeof(Vec2), nullptr,
      Werwel::VertexBufferLayouts { { 2, sizeof(Vec2), 0, 1 } }
    );

  ImageAsset lightTexture("Assets/Images/LightMask16.png");
  lightMesh.texture = CreateRef<Werwel::Texture>(
    Werwel::Size { lightTexture.GetSize().x, lightTexture.GetSize().y },
    lightTexture.GetData(),
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER,          GL_LINEAR },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER,          GL_LINEAR },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int,     GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int,     GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER }
  );

  /**
   * VERY IMPORTANT!
   */
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void LightPass::Perform(const Ref<Camera>& camera, int amountOfLights) {  
  NATURAFORGE_PROFILER_SCOPE();  

	Mat4 projView = Window::GetSpace() * camera->GetTransform();

  fbo->Bind();
  fbo->Clear();
    shader->Bind();
    shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
      lightMesh.vao->Bind();
      lightMesh.vao->GetIndexBuffer()->Bind();
        lightMesh.texture->Bind();
          glDrawElementsInstanced(GL_TRIANGLES, lightMesh.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfLights);
  fbo->Unbind();

  NATURAFORGE_SYNC_GPU();
}