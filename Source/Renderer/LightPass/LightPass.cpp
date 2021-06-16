#include "LightPass.h"

LightPass::LightPass() {
  fbo = CreateRef<LightFBO>(Window::GetSize());

  TextAsset vsCode("Assets/Shaders/Terrain/LightPassShader.vs");
  TextAsset fsCode("Assets/Shaders/Terrain/LightPassShader.fs");
  shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_Proj", "u_View");
    
  const auto& vertices = Primitives::Block::Vertices(200, 200);
  const auto& indices = Primitives::Block::indices;

  lightMesh.vao = CreateRef<Werwel::VAO>();
  lightMesh.vao->Bind();
    lightMesh.vao->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
    lightMesh.vao->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
    vbo = lightMesh.vao->AddVBO(
      Werwel::VBO::Type::Array, Werwel::VBO::Usage::Stream, 10000, sizeof(Vec2), nullptr,
      Werwel::VertexBufferLayouts { { 2, sizeof(Vec2), 0, 1 } }
    );

  ImageAsset lightTexture("Assets/Images/LightMask32.png");
  lightMesh.texture = CreateRef<Werwel::Texture>(
    Werwel::Size { lightTexture.GetSize().x, lightTexture.GetSize().y },
    lightTexture.GetData(),
    GL_RGBA,
    GL_RGBA,
    GL_UNSIGNED_BYTE,
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_NEAREST },
    Werwel::Texture::param_t { Werwel::Texture::ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_NEAREST }
  );

  /**
   * VERY IMPORTANT!
   */
  glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
}

void LightPass::Perform(const Ref<Camera>& camera, int amountOfBlocks) {  
  FORGIO_PROFILER_SCOPE();

  {
    FORGIO_PROFILER_NAMED_SCOPE("Binding");
    glClearColor(0, 0, 0, 1);
    fbo->Bind();
    fbo->Clear();
      shader->Bind();
      shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
      shader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));
        lightMesh.vao->Bind();
        lightMesh.vao->GetIndexBuffer()->Bind();
          lightMesh.texture->Bind();

    FORGIO_SYNC_GPU();
  }

  {
    FORGIO_PROFILER_NAMED_SCOPE("Rendering");

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr, amountOfBlocks);

    FORGIO_SYNC_GPU();
  }
          
    shader->Unbind();
  fbo->Unbind();

  FORGIO_SYNC_GPU();
}