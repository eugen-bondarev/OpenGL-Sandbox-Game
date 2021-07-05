#include "light_pass.h"

#include "core/window.h"

LightPass::LightPass()
{
	fbo = CreateRef<LightFBO>(Vec2(300, 168));

	ww::TextAsset vsCode("assets/shaders/terrain/light_pass_shader.vs", NF_ROOT);
	ww::TextAsset fsCode("assets/shaders/terrain/light_pass_shader.fs", NF_ROOT);
	shader = CreateRef<ww::Shader>(vsCode.GetContent(), fsCode.GetContent(), ww::Uniforms { "u_ProjectionView" });

	/**
	 * Amount of blocks that each light reaches.
	 */
	float lightBlocks = 3.5f;

	/**
	 * Empirical constant.
	 */
	const float lightTextureGradientConstant = 1.55f;
	const auto &vertices = Primitives::Block::Vertices(
		16 * (lightBlocks * lightTextureGradientConstant) * 2,
		16 * (lightBlocks * lightTextureGradientConstant) * 2);
	const auto &indices = Primitives::Block::indices;

	lightMesh.vao = CreateRef<ww::VAO>();
	lightMesh.vao->Bind();
	lightMesh.vao->AddVBO(ww::VBO::Type::Array, ww::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
	lightMesh.vao->AddVBO(ww::VBO::Type::Indices, ww::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
	vbo = lightMesh.vao->AddVBO(
		ww::VBO::Type::Array, ww::VBO::Usage::Stream, 0, sizeof(Vec2), nullptr,
		ww::VertexBufferLayouts{{2, sizeof(Vec2), 0, 1}});

	ww::ImageAsset lightTexture("assets/images/light_mask_32.png", NF_ROOT);
	lightMesh.texture = CreateRef<ww::Texture>(
		lightTexture.GetSize(),
		lightTexture.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		ww::Texture::Parameters_t{
			ww::Texture::SetInterpolation(ww::Interpolation::Linear)});

	/**
	 * VERY IMPORTANT!
	 */
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

	// Window::callbacks.push_back([&]() {
	// 	LOG_OUT(Window::GetSize().x);
	// 	LOG_OUT(Window::GetSize().y);
	// 	fbo->Resize(Window::GetSize());
	// });
}

void LightPass::Perform(const Ref<Camera> &camera, int amountOfLights)
{
	NF_PROFILER_SCOPE();

	glViewport(0, 0, 300, 168);

	Mat4 projView = ww::Window::GetSpace() * camera->GetTransform();

	fbo->Bind();
	fbo->Clear();
	shader->Bind();
	shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
	lightMesh.vao->Bind();
	lightMesh.vao->GetIndexBuffer()->Bind();
	lightMesh.texture->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, lightMesh.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfLights);
	fbo->Unbind();

	glViewport(0, 0, ww::Window::GetSize().x, ww::Window::GetSize().y);

	NF_SYNC_GPU();
}