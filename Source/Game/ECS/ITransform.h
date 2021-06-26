#pragma once

#include "Maths/Maths.h"

class ITransform {
friend class Rigidbody;

public:
  inline Vec2 GetPosition() const {
    return position;
  }

  template <typename... Args>
  inline void SetPosition(Args&&... args) {
    position = Vec2(std::forward<Args>(args)...);
    CalculateTransform();
  }

  inline void SetPositionY(float y) {
    position.y = y;
    CalculateTransform();
  }

  inline void SetPositionX(float x) {
    position.x = x;
    CalculateTransform();
  }

  template <typename... Args>
  inline void AddPosition(Args... args) {
    Vec2 delta = Vec2(std::forward<Args>(args)...);
    SetPosition(position + delta);
  }

  inline const Mat4& GetTransform() const {
    return transform;
  }

  virtual void CalculateTransform() = 0;

protected:
  Vec2 position;

  Mat4 transform;
};