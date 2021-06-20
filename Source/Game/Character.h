#pragma once

#include "../Maths/Maths.h"
#include "ITransform.h"
#include "Map.h"

#define LINOW_USE_GLM
#include "Linow/Linow.h"

class Character : public ITransform {
public:
  inline static constexpr float velocityFactor = 150.0f;

  Character(const Ref<Map>& map);

  Blocks::Representations::IndexAndPosition GetBlockNearby(Vec2 fix, Vec2 offset = Vec2(0, 0)) const;

  void CollectLights(std::vector<Vec2>& lights) const;
  void Update(float deltaTime);
  void CheckCollisions();
  void Jump();

  void SetCeiling(bool value);
  void SetOnGround(bool value);
  void CanMoveLeft(bool value);
  void CanMoveRight(bool value);

  bool GetCeiling() const;
  bool GetOnGround() const;
  bool CanMoveLeft() const;
  bool CanMoveRight() const;

private:
  const Ref<Map>& map;

  bool ceiling      { false };
  bool onGround     { false };
  bool canMoveLeft  { true };
  bool canMoveRight { true };

  Vec2 velocity { 0 };

  void CalculateTransform() override {
    transform = Math::Translate(Mat4(1), Vec3(position, 0.0));
  }

  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;
};