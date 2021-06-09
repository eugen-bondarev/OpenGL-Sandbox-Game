#pragma once

#include "Werwel/Shader.h"
#include "Werwel/VAO.h"

struct Line {
  std::array<Vec2, 2> points;
  Color color { 1, 0, 0, 1 };
  
  inline Line(const Vec2& start, const Vec2& end, const Color& color = Color(1.0f, 0.0f, 0.0f, 1.0f)) {
    points[0] = start;
    points[1] = end;
    this->color = color;
  }
};

class DebugRenderer {
public:
  DebugRenderer();

  void Render(const Mat4& viewMatrix);

  inline void ClearLines() {
    lines.clear();
  }

  template <typename... Args>
  inline void AddLine(Args&&... args) {
    lines.emplace_back(std::forward<Args>(args)...);
  }

private:
  std::vector<Line> lines;
  Ptr<Werwel::Shader> lineShader;
};