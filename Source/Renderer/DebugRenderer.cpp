#include "DebugRenderer.h"

#include "Assets/TextAsset.h"

#include "Math/Primitive.h"

#include "Core/Window.h"

struct LineVertex {
	Vec2 position;

	inline static Werwel::VertexBufferLayouts GetLayout() {
		return {
			{ 2, sizeof(LineVertex), offsetof(LineVertex, position) }
		};
	}
};

DebugRenderer::DebugRenderer() {
  TextAsset lineVS("Assets/Shaders/Debug/Line.vs");
  TextAsset lineFS("Assets/Shaders/Debug/Line.fs");
  lineShader = CreatePtr<Werwel::Shader>(lineVS.GetContent(), lineFS.GetContent(), "u_Proj", "u_View", "u_LineData");
  lineShader->Bind();
    lineShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));
}

void DebugRenderer::Render(const Mat4& viewMatrix) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  lineShader->Bind();
  lineShader->SetMat4x4("u_View", Math::ToPtr(viewMatrix));
  lineShader->SetMat4x4("u_Proj", Math::ToPtr(Window::GetSpace()));

  for (const Line& line : lines) {
    Mat2x4 lineData;

    memcpy(&lineData[0][0], &line.points[0].x, sizeof(Vec4));
    memcpy(&lineData[1][0], &line.color.x, sizeof(Vec4));

    lineShader->SetMat2x4("u_LineData", Math::ToPtr(lineData));
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}