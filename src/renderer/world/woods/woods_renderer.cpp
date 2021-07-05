#include "woods_renderer.h"

#include "assets/text_asset.h"
#include "assets/image_asset.h"

#include "maths/primitive.h"

WoodsRenderer::WoodsRenderer(const Ref<Woods> &woods, const Ref<Camera> &camera) : woods{woods}, camera{camera}
{
	ww::TextAsset vsCode("assets/shaders/woods/vs_woods.glsl", NF_ROOT);
	ww::TextAsset fsCode("assets/shaders/woods/fs_woods.glsl", NF_ROOT);

	pipeline.shader = CreateRef<ww::Shader>(vsCode.GetContent(), fsCode.GetContent(), ww::Uniforms { "u_ProjectionView", "u_Model" });

	const auto &vers = Primitives::Block::Vertices(16, 16);
	const auto &inds = Primitives::Block::indices;

	pipeline.barkVAO = CreateRef<ww::VAO>();
	pipeline.barkVAO->Bind();
	pipeline.barkVAO->AddVBO(ww::VBO::Type::Array, ww::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
	pipeline.barkVAO->AddVBO(ww::VBO::Type::Indices, ww::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);
	pipeline.vbo = pipeline.barkVAO->AddVBO(
		ww::VBO::Type::Array,
		ww::VBO::Usage::Stream,
		0,
		sizeof(Vec2),
		nullptr,
		std::vector<ww::VertexBufferLayout>{
			{2, sizeof(Vec2), 0, 1}});

	std::vector<Vec2> positions;

	for (int i = 0; i < woods->GetTrees().size(); i++)
	{
		positions.push_back(woods->GetTrees()[i].GetPosition());
	}

	pipeline.vbo->Bind();
	pipeline.vbo->Store(positions);

	const ww::ImageAsset tileMapTexture("assets/images/bark.png", NF_ROOT);
	pipeline.barkTexture = CreateRef<ww::Texture>(
		tileMapTexture.GetSize(),
		tileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		ww::Texture::Parameters_t{
			ww::Texture::SetInterpolation(ww::Interpolation::Constant)});

	const ww::ImageAsset leavesTextureAsset("assets/images/leaves_stroke.png", NF_ROOT);
	pipeline.leavesTexture = CreateRef<ww::Texture>(
		leavesTextureAsset.GetSize(),
		leavesTextureAsset.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE,
		ww::Texture::Parameters_t{
			ww::Texture::SetInterpolation(ww::Interpolation::Constant)});

	barkModelMatrix = Math::Scale(Mat4(1), Vec3(barkSize / 16.0f, 1.0f));

	leavesModelMatrix = Math::Translate(Mat4(1), Vec3(8, 16 * 3, 0));
	leavesModelMatrix = Math::Scale(leavesModelMatrix, Vec3(leavesSize / 16.0f, 1.0f));

	woods->GetVisibleTrees(visibleTrees, camera);
}

void WoodsRenderer::Render()
{
	Mat4 projView = ww::Window::GetSpace() * camera->GetTransform();

	visibleTrees.clear();
	woods->GetVisibleTrees(visibleTrees, camera);
	pipeline.vbo->Bind();
	pipeline.vbo->Store(visibleTrees);

	pipeline.shader->Bind();
	pipeline.shader->SetMat4x4("u_ProjectionView", Math::ToPtr(projView));
	pipeline.shader->SetMat4x4("u_Model", Math::ToPtr(barkModelMatrix));
	pipeline.barkVAO->Bind();
	pipeline.barkVAO->GetIndexBuffer()->Bind();
	pipeline.barkTexture->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, pipeline.barkVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, visibleTrees.size());

	pipeline.shader->SetMat4x4("u_Model", Math::ToPtr(leavesModelMatrix));
	pipeline.leavesTexture->Bind();
	glDrawElementsInstanced(GL_TRIANGLES, pipeline.barkVAO->GetIndexBuffer()->GetIndexCount(), GL_UNSIGNED_INT, nullptr, visibleTrees.size());
}