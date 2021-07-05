#include "composition_pass.h"

CompositionPass::CompositionPass()
{
	ww::TextAsset vsCode("assets/shaders/terrain/composition_shader.vs", NF_ROOT);
	ww::TextAsset fsCode("assets/shaders/terrain/composition_shader.fs", NF_ROOT);
	shader = CreateRef<ww::Shader>(vsCode.GetContent(), fsCode.GetContent(), ww::Uniforms { "u_ColorPassResult", "u_LightPassResult" });
	shader->Bind();
	shader->SetInt("u_ColorPassResult", 0);
	shader->SetInt("u_LightPassResult", 1);
	shader->Unbind();

	ww::TextAsset vsSky("assets/shaders/sky/sky.vs", NF_ROOT);
	ww::TextAsset fsSky("assets/shaders/sky/sky.fs", NF_ROOT);
	sky.shader = CreateRef<ww::Shader>(vsSky.GetContent(), fsSky.GetContent(), ww::Uniforms { "u_Color0", "u_Color1" });
	sky.shader->Bind();
	sky.shader->SetVec3("u_Color0", Math::ToPtr(Vec3(176, 226, 255) / 255.0f));
	sky.shader->SetVec3("u_Color1", Math::ToPtr(Vec3(99, 172, 255) / 255.0f));
	sky.shader->Unbind();

	const auto &vers = Primitives::Block::Vertices(2, -2);
	const auto &inds = Primitives::Block::indices;

	canvas = CreateRef<ww::VAO>();
	canvas->Bind();
	canvas->AddVBO(ww::VBO::Type::Array, ww::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
	canvas->AddVBO(ww::VBO::Type::Indices, ww::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
}

void CompositionPass::Perform(const Ref<ColorPass> &colorPass, const Ref<LightPass> &lightPass)
{
	NF_PROFILER_SCOPE();

	ww::GraphicsContext::Clear();

	canvas->Bind();
	canvas->GetIndexBuffer()->Bind();
	sky.shader->Bind();
	glDrawElements(GL_TRIANGLES, canvas->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	shader->Bind();
	colorPass->GetFBO()->GetTexture()->Bind(GL_TEXTURE0);
	lightPass->GetFBO()->GetTexture()->Bind(GL_TEXTURE0 + 1);
	glDrawElements(GL_TRIANGLES, canvas->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	glActiveTexture(GL_TEXTURE0);

	NF_SYNC_GPU();
}