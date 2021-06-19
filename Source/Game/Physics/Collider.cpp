#include "Collider.h"

Collider::Collider(Vec2& position, ColliderRect rect) : position { position }, rect { rect } {
  
}

Vec2 Collider::GetStart() const {
  return position + Vec2(rect.x.left, 16 * 4 - rect.y.top);
}

Vec2 Collider::GetEnd() const {
  return position + Vec2(16 * 3 - rect.x.right, rect.y.bottom);
}