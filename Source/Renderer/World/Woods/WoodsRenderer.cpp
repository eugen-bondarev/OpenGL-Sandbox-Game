#include "WoodsRenderer.h"

#include "Assets/TextAsset.h"
#include "Assets/ImageAsset.h"

#include "Maths/Primitive.h"

WoodsRenderer::WoodsRenderer(const Ref<Woods>& woods, const Ref<Camera>& camera) : woods { woods }, camera { camera } {
	TextAsset vsCode("Assets/Shaders/Woods/VS_Woods.glsl");
	TextAsset fsCode("Assets/Shaders/Woods/FS_Woods.glsl");

  	pipeline.shader = CreateRef<Werwel::Shader>(vsCode.GetContent(), fsCode.GetContent(), "u_ProjectionView", "u_Model");

	const auto& vers = Primitives::Block::Vertices(16, 16);
	const auto& inds = Primitives::Block::indices;

	pipeline.barkVAO = CreateRef<Werwel::VAO>();
	pipeline.barkVAO->BindSafely();
		pipeline.barkVAO->AddVBO(Werwel::VBO::Type::Array, Werwel::VBO::Usage::Static, vers.size(), sizeof(Vertex2D), &vers[0], Vertex2D::GetLayout());
		pipeline.barkVAO->AddVBO(Werwel::VBO::Type::Indices, Werwel::VBO::Usage::Static, inds.size(), sizeof(int), &inds[0]);		
		pipeline.vbo = pipeline.barkVAO->AddVBO(
			Werwel::VBO::Type::Array, 
			Werwel::VBO::Usage::Stream, 
			0, 
			sizeof(Vec2), 
			nullptr, 
			std::vector<Werwel::VertexBufferLayout> { 
				{ 2, sizeof(Vec2), 0, 1 }
			}
		);

  	std::vector<Vec2> positions;

	for (int i = 0; i < woods->GetTrees().size(); i++) {
		positions.push_back(woods->GetTrees()[i].GetPosition());
	}

  	pipeline.vbo->Bind();
    pipeline.vbo->Store(positions);

	const ImageAsset tileMapTexture("Assets/Images/Bark.png");
	pipeline.barkTexture = CreateRef<Werwel::Texture>(
		tileMapTexture.GetSize(),
		tileMapTexture.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 
		Werwel::Texture::Parameters_t {
			Werwel::Texture::SetInterpolation(Werwel::Interpolation::Constant)
		}
	);

	const ImageAsset leavesTextureAsset("Assets/Images/Leaves_Stroke.png");
	pipeline.leavesTexture = CreateRef<Werwel::Texture>(
		leavesTextureAsset.GetSize(),
		leavesTextureAsset.GetData(),
		GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, 
		Werwel::Texture::Parameters_t {
			Werwel::Texture::SetInterpolation(Werwel::Interpolation::Constant)
		}
	);

	barkModelMatrix = Math::Scale(Mat4(1), Vec3(barkSize / 16.0f, 1.0f));

	leavesModelMatrix = Math::Translate(Mat4(1), Vec3(8, 16 * 3, 0));
	leavesModelMatrix = Math::Scale(leavesModelMatrix, Vec3(leavesSize / 16.0f, 1.0f));
	
  	woods->GetVisibleTrees(visibleTrees, camera);
}

void WoodsRenderer::Render() {
	Mat4 projView = Window::GetSpace() * camera->GetTransform();

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