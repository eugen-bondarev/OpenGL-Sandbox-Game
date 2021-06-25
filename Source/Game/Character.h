#pragma once

#include "../Maths/Maths.h"

#include "Components/Animator.h"
#include "Components/Rigidbody.h"
#include "Components/Entity.h"

#include "Map.h"

class Character : public Entity {
public:
  Character(const Ref<Map>& map);
  void CollectLights(std::vector<Vec2>& lights) const;

private:
  const Ref<Map>& map;

  void CalculateTransform() override {
    transform = Math::Translate(Mat4(1), Vec3(position, 0.0));
  }

  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;
};