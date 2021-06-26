#pragma once

#include "../../Maths/Maths.h"

#include "../ECS/Animator.h"
#include "../ECS/Rigidbody.h"
#include "../ECS/Player.h"
#include "../ECS/Entity.h"

#include "../World.h"

class Character : public Entity {
public:
  Character(const Ref<World>& world);
  void CollectLights(std::vector<Vec2>& lights) const;
  void Land();

private:
  const Ref<World>& world;

  void CalculateTransform() override {
    transform = Math::Translate(Mat4(1), Vec3(position, 0.0));
  }

  Character(const Character&) = delete;
  Character& operator=(const Character&) = delete;
};