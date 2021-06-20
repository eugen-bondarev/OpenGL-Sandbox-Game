#pragma once

struct ColliderRect {
  struct {
    float left;
    float right;
  } x;

  struct {
    float top;
    float bottom;
  } y;
};

class Collider {
public:
  Collider(ColliderRect rect, Vec2 size);

  void SetPosition(const Vec2& position);
  bool IntersectsWith(const Collider& collider) const;

  Vec2 GetStart() const;
  Vec2 GetEnd() const;

private:
  Vec2 position;
  ColliderRect rect;
  Vec2 size;

  Vec2 start;
  Vec2 end;
};