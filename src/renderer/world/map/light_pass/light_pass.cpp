#include "light_pass.h"

LightPass::LightPass()
{
	fbo = CreateRef<LightFBO>(Vec2(300, 168));

	mw::TextAsset vsCode("assets/shaders/terrain/light_pass_shader.vs");
	mw::TextAsset fsCode("assets/shaders/terrain/light_pass_shader.fs");
	shader = CreateRef<mw::Shader>(vsCode.GetContent(), fsCode.GetContent(), mw::Uniforms { "u_ProjectionView" });

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

	lightMesh.vao = CreateRef<mw::VAO>();
	lightMesh.vao->Bind();
	lightMesh.vao->AddVBO(mw::VBO::Type::Array, mw::VBO::Usage::Static, vertices.size(), sizeof(Vertex2D), &vertices[0], Vertex2D::GetLayout());
	lightMesh.vao->AddVBO(mw::VBO::Type::Indices, mw::VBO::Usage::Static, indices.size(), sizeof(int), &indices[0]);
	vbo = lightMesh.vao->AddVBO(
		mw::VBO::Type::Array, mw::VBO::Usage::Stream, 0, sizeof(Vec2), nullptr,
		mw::VertexBufferLayouts{{2, sizeof(Vec2), 0, 1}});

	mw::ImageAsset lightTexture("assets/images/light_mask_32.png");
	lightMesh.texture = CreateRef<mw::Texture>(
		lightTexture.GetSize(),
		lightTexture.GetData(),
		GL_RGBA,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		mw::Texture::Parameters_t{
			mw::Texture::SetInterpolation(mw::Interpolation::Linear)});

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
	MW_PROFILER_SCOPE();

	glViewport(0, 0, 300, 168);

	Mat4 projView = mw::Window::GetSpace() * camera->GetTransform();

	fbo->Bind();
	fbo->Clear();
	shader->Bind();
	shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
	lightMesh.vao->Bind();
	lightMesh.vao->GetIndexBuffer()->Bind();
	lightMesh.texture->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, lightMesh.vao->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, amountOfLights);
	fbo->Unbind();

	glViewport(0, 0, mw::Window::GetSize().x, mw::Window::GetSize().y);

	MW_SYNC_GPU();
}