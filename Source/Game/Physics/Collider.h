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
  Collider(Vec2& position, ColliderRect rect);

  Vec2 GetStart() const;
  Vec2 GetEnd() const;

private:
  const Vec2& position;
  ColliderRect rect;

  Vec2 start;
  Vec2 end;
};