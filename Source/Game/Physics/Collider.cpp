#include "Collider.h"

Collider::Collider(ColliderRect rect, Vec2 size) : rect { rect }, size { size } {
  
}

void Collider::SetPosition(const Vec2& position) {
  this->position = position;
}

Vec2 Collider::GetStart() const {
  return position + Vec2(rect.x.left, rect.y.bottom);
}

Vec2 Collider::GetEnd() const {
  return position + Vec2(size.x - rect.x.right, size.y - rect.y.top);
}

void LogOut(const Vec2& vec) {
  LOG_OUT(vec.x << ' ' << vec.y);
}

inline static bool PointIntersects(const Vec2& point, const Collider& collider) {
  // LOG_OUT("vec");
  // LogOut(point);
  // LOG_OUT("col start");
  // LogOut(collider.GetStart());
  // LOG_OUT("col end");
  // LogOut(collider.GetEnd());

  return point.x >= collider.GetStart().x && point.x <= collider.GetEnd().x && point.y <= collider.GetStart().y && point.y >= collider.GetEnd().y;
}

bool Collider::IntersectsWith(const Collider& collider) const {
  Vec2 p0 = Vec2(GetStart().x, GetStart().y);
  Vec2 p1 = Vec2(GetEnd().x, GetStart().y);
  Vec2 p2 = Vec2(GetStart().x, GetEnd().y);
  Vec2 p3 = Vec2(GetEnd().x, GetEnd().y);

  Vec2 s0 = Vec2(collider.GetStart().x, collider.GetStart().y);
  Vec2 s1 = Vec2(collider.GetEnd().x, collider.GetStart().y);
  Vec2 s2 = Vec2(collider.GetStart().x, collider.GetEnd().y);
  Vec2 s3 = Vec2(collider.GetEnd().x, collider.GetEnd().y);

  bool in0 = PointIntersects(p0, collider)
      || PointIntersects(p1, collider)
      || PointIntersects(p2, collider)
      || PointIntersects(p3, collider);

  bool in1 = PointIntersects(s0, collider)
      || PointIntersects(s1, collider)
      || PointIntersects(s2, collider)
      || PointIntersects(s3, collider);

  return in0 || in1;
}