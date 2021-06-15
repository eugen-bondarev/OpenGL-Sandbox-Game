#pragma once

#include "ColorFBO.h"

#include "Werwel/Texture.h"
#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

#include "Game/Control/Camera.h"

class ColorPass {
public:
  ColorPass(int amountOfBlocks);
  const Ref<ColorFBO>& GetFBO() const;

  Ref<ColorFBO> fbo;
  Ref<Werwel::Shader> shader;
  Ref<Werwel::VAO> vao;
  Ref<Werwel::VBO> vbo;
  Ref<Werwel::Texture> tileMap;

  void Perform(const Ref<Camera>& camera, int amountOfBlocks) {
    {
      FORGIO_PROFILER_NAMED_SCOPE("Binding");
      shader->Bind();
      shader->SetMat4x4("u_View", Math::ToPtr(camera->GetViewMatrix()));
      tileMap->BindSafely();
      vao->BindSafely();
      vao->GetIndexBuffer()->BindSafely();
      FORGIO_SYNC_GPU();
    }

    {
      FORGIO_PROFILER_NAMED_SCOPE("Rendering");
      fbo->Bind();
      fbo->Clear();
      glDrawElementsInstanced(GL_TRIANGLES, vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfBlocks);
      fbo->Unbind();
      FORGIO_SYNC_GPU();
    }
  }
};