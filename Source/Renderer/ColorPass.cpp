#include "ColorPass.h"

#include "Assets/TextAsset.h"
#include "Core/Window.h"

#include "Gpu/GraphicsContext.h"

ColorPass::ColorPass() {
    fbo = std::make_shared<ColorFbo>(Window::GetSize());

    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Terrain/ColorPass.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Terrain/ColorPass.fs");
    } shaders;

    shader = std::make_shared<Shader>(shaders.vertex.GetContent(), shaders.fragment.GetContent(), "u_Proj", "u_View", "u_Pos", "u_AmountOfTiles", "u_Offset");
    shader->Bind();
        shader->SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 5.0f)));
        shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    shader->Unbind();
}

void ColorPass::Execute(LightData& lightData, const Mat4& viewMatrix, const Vec2& viewPos, std::shared_ptr<Map>& map) {    
    GraphicsContext::ClearColor({ 0, 0, 0, 0 });
    
    fbo->Bind();
    fbo->Clear();
        shader->Bind();
        shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
            lightData = map->Render(shader, viewPos);
        shader->Unbind();
    fbo->Unbind();
}