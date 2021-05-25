#include "CompositionPass.h"

#include "Core/Window.h"

#include "Assets/TextAsset.h"

#include "Math/Primitive.h"

#include "Gpu/GraphicsContext.h"

CompositionPass::CompositionPass() {    
    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Terrain/Composition.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Terrain/Composition.fs");
    } shaders;

    shader = std::make_unique<Shader>(
        shaders.vertex.GetContent(), 
        shaders.fragment.GetContent(), 
        "u_Proj", 
        "u_ColorPassResult", 
        "u_LightPassResult"
    );

    shader->Bind();
        shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
        shader->SetInt("u_ColorPassResult", 0);
        shader->SetInt("u_LightPassResult", 1);
    shader->Unbind();

    canvas = std::make_unique<Vao>(Primitives::Canvas::vertices, Vertex::GetLayout(), Primitives::Canvas::indices);
}

void CompositionPass::Execute(std::shared_ptr<ColorPass>& colorPass, std::shared_ptr<LightPass>& lightPass) {
    GraphicsContext::ClearColor({ 219.0f / 255.0f, 249.0f / 255.0f, 255.0f / 255.0f, 1.0f});
    shader->Bind();
        canvas->Bind();
            colorPass->GetFbo()->BindTexture(GL_TEXTURE0);
            lightPass->GetFbo()->BindTexture(GL_TEXTURE0 + 1);
                glDrawElements(GL_TRIANGLES, canvas->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            lightPass->GetFbo()->UnbindTexture(GL_TEXTURE0 + 1);
            colorPass->GetFbo()->UnbindTexture(GL_TEXTURE0);
        canvas->Unbind();
    shader->Unbind();
}