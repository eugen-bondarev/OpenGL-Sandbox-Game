#pragma once

#include "../Math/Math.h"

class Character {
public:
  Character() = default;

  inline Vec2 GetPosition() const {
    return position;
  }

  template <typename... Args>
  inline void SetPosition(Args&&... args) {
    position = Vec2(std::forward<Args>(args)...);
    CalculateModelMatrix();
  }

  inline void SetPositionY(float y) {
    position.y = y;
    CalculateModelMatrix();
  }

  template <typename... Args>
  inline void AddPosition(Args... args) {
    Vec2 delta = Vec2(std::forward<Args>(args)...);
    SetPosition(position + delta);
  }

  inline const Mat4& GetModelMatrix() const {
    return modelMatrix;
  }

  inline void Update(float deltaTime) {
    if (ceiling) {
      acceleration = 0;
      ceiling = false;
    }

    if (!onGround) {
      SetPosition(position + Vec2(0.0f, deltaTime) * acceleration);
      acceleration += -9.81f;
    } else {
      acceleration = 0;
    }
  }

  inline void Jump() {
    acceleration = 320.0f;
    onGround = false;
  }

  bool ceiling = false;
  bool onGround = true;
  bool canMoveRight = true;
  bool canMoveLeft = true;

  float acceleration { 0 };

private:
  Vec2 position;
  Mat4 modelMatrix;

  inline void CalculateModelMatrix() {
    modelMatrix = Math::Translate(Mat4(1), Vec3(position, 0.0));
  }

  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;
};