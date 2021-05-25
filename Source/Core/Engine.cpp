#include "Engine.h"

#include "Core/Window.h"
#include "Core/Gui.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"
#include "Math/Primitive.h"

Engine::Engine() {
    Window::Create();
    Gui::Create();
}

void Engine::InitResources() {
    map = std::make_unique<Map>(Size { 2000, 1000 }, 16);

    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Terrain/ColorPass.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Terrain/ColorPass.fs");
    } colorPassShaders;

    colorPass.shader = std::make_unique<Shader>(colorPassShaders.vertex.GetContent(), colorPassShaders.fragment.GetContent(), "u_Proj", "u_View", "u_Pos", "u_AmountOfTiles", "u_Offset");
    colorPass.shader->Bind();
        colorPass.shader->SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 3.0f)));
        colorPass.shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    colorPass.shader->Unbind();

    colorPass.fbo = std::make_unique<ColorFbo>(Window::GetSize());

    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Terrain/LightPass.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Terrain/LightPass.fs");
    } lightPassShaders;

    lightPass.shader = std::make_unique<Shader>(lightPassShaders.vertex.GetContent(), lightPassShaders.fragment.GetContent(), "u_Proj", "u_View", "u_Model", "u_Pos", "u_AmountOfTiles", "u_Offset");
    lightPass.shader->Bind();
        lightPass.shader->SetVec2("u_AmountOfTiles", Math::ToPtr(Vec2(9.0f, 3.0f)));
        lightPass.shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    lightPass.shader->Unbind();

    lightPass.fbo = std::make_unique<LightFbo>(Window::GetSize());

    // Light pass
    lightVao = std::make_unique<Vao>(Primitives::Quad::vertices, QuadVertex::GetLayout(), Primitives::Quad::indices);
    ImageAsset lightImage("Assets/Images/LightMask5.png");
    lightTexture = std::make_unique<Texture>(
        lightImage.GetSize(),
        lightImage.GetData(),
        GL_RGBA,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        std::vector<Texture::param_t> {
            { ParamType::Int, GL_TEXTURE_MIN_FILTER, GL_LINEAR },
            { ParamType::Int, GL_TEXTURE_MAG_FILTER, GL_LINEAR }
        }
    );
    // Light pass

    // Composition pass
    struct {
        TextAsset vertex = TextAsset("Assets/Shaders/Terrain/Composition.vs");
        TextAsset fragment = TextAsset("Assets/Shaders/Terrain/Composition.fs");
    } compositionShaders;

    composition.shader = std::make_unique<Shader>(compositionShaders.vertex.GetContent(), compositionShaders.fragment.GetContent(), "u_Proj", "u_ColorPassResult", "u_LightPassResult");
    composition.shader->Bind();
        GLuint decalTexLocation = glGetUniformLocation(composition.shader->GetHandle(), "u_ColorPassResult");
        GLuint bumpTexLocation  = glGetUniformLocation(composition.shader->GetHandle(), "u_LightPassResult");
        
        glUniform1i(decalTexLocation, 0);
        glUniform1i(bumpTexLocation,  1);

        composition.shader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
    composition.shader->Unbind();

    composition.canvas = std::make_unique<Vao>(Primitives::Quad::canvas, QuadVertex::GetLayout(), Primitives::Quad::indices);
    // Composition pass
}

bool Engine::IsRunning() const {
    return !Window::ShouldClose();
}

void Engine::BeginFrame() {
    Window::Clear();
    Window::PollEvents();
    Gui::Begin();
}

void Engine::Control() {
    if (Window::KeyPressed(GLFW_KEY_W)) viewPos += Vec2( 0,  1) * 4.0f;
    if (Window::KeyPressed(GLFW_KEY_S)) viewPos += Vec2( 0, -1) * 4.0f;
    if (Window::KeyPressed(GLFW_KEY_A)) viewPos += Vec2(-1,  0) * 4.0f;
    if (Window::KeyPressed(GLFW_KEY_D)) viewPos += Vec2( 1,  0) * 4.0f;
}

void Engine::Render() {
    viewMatrix = Math::Inverse(Math::Translate(Mat4(1), Vec3(viewPos, 0.0f)));

    glClearColor(0, 0, 0, 0.0f);
    colorPass.fbo->Bind();
    colorPass.fbo->Clear();
        colorPass.shader->Bind();
        colorPass.shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
            LightData lightData = map->Render(*colorPass.shader, viewPos);
        colorPass.shader->Unbind();
    colorPass.fbo->Unbind();

    lightPass.fbo->Bind();
    lightPass.fbo->Clear();
        lightPass.shader->Bind();
        lightPass.shader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));

            lightVao->Bind();
            lightTexture->Bind();

            for (int i = 0; i < lightData.blocksThrowingLight.size(); i++) {
                Mat4 lightModelMatrix = Math::Translate(Mat4(1), Vec3(lightData.blocksThrowingLight[i].pos, 0.0f));
                lightModelMatrix = Math::Scale(lightModelMatrix, Vec3(15));
                lightPass.shader->SetMat4x4("u_Model", Math::ToPtr(lightModelMatrix));
                glDrawElements(GL_TRIANGLES, lightVao->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
            }

            lightTexture->Unbind();
            lightVao->Unbind();

        lightPass.shader->Unbind();
    lightPass.fbo->Unbind();

    glClearColor(219 / 255.0f, 249 / 255.0f, 255 / 255.0f, 1.0f);
    composition.shader->Bind();
        composition.canvas->Bind();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, colorPass.fbo->GetTextureHandle());
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, lightPass.fbo->GetTextureHandle());

                glDrawElements(GL_TRIANGLES, composition.canvas->GetVertexCount(), GL_UNSIGNED_INT, nullptr);
                
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, 0);

        composition.canvas->Unbind();
    composition.shader->Unbind();
}

void Engine::EndFrame() {
    Gui::End();
    Window::SwapBuffers();
}

Engine::~Engine() {
    Gui::Destroy();
    Window::Destroy();
}