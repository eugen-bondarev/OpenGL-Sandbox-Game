#pragma once

#include "Component.h"

#include "ITransform.h"

#include "../Map.h"

#include "Entity.h"

class Rigidbody : public Component {
public:
  inline static constexpr float velocityFactor = 150.0f;

  Blocks::Representations::IndexAndPosition GetBlockNearby(Vec2 fix, Vec2 offset = Vec2(0, 0)) const;

  Rigidbody(const Ref<Entity> entity, const Ref<Map> map) : Component(entity), map { map } {

  }

  void Update();
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
  const Ref<Map> map;

  bool ceiling      { false };
  bool onGround     { false };
  bool canMoveLeft  { true };
  bool canMoveRight { true };

  Vec2 velocity { 0 };
};