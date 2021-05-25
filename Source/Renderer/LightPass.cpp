#include "LightPass.h"

#include "Assets/TextAsset.h"
#include "Core/Window.h"

#include "Entities/RectVao.h"

LightPass::LightPass() {
    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Terrain/LightPass.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Terrain/LightPass.fs");
    } shaders;

    shader = std::make_unique<Shader>(shaders.vertex.GetContent(), shaders.fragment.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos");
    shader->Bind();
        shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    shader->Unbind();

    fbo = std::make_unique<LightFbo>(Window::GetSize());    

    ImageAsset lightImage("Assets/Images/LightMask5.png");

    lightSprite = std::make_unique<Sprite>(lightImage);
    lightSprite->SetScale(7);
}

void LightPass::Execute(const LightData& lightData, const Mat4& viewMatrix) {
    fbo->Bind();
    fbo->Clear();
        shader->Bind();
        shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));

            Primitives::Rect::vao->Bind();
            lightSprite->GetTexture()->Bind();

            for (int i = 0; i < lightData.blocksThrowingLight.size(); i++) {
                lightSprite->SetPosition(lightData.blocksThrowingLight[i].position);
                shader->SetMat4x4("u_Model", Math::ToPtr(lightSprite->GetTransformation()));
                Primitives::Rect::Draw();
            }

            lightSprite->GetTexture()->Unbind();
            Primitives::Rect::vao->Unbind();

        shader->Unbind();
    fbo->Unbind();
}