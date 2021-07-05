#pragma once

#include "component.h"

#include "animation/animation.h"
#include "animation/humanoid/walking.h"
#include "animation/humanoid/attacking.h"

class Entity;

class Animator : public Component {
public:
  Animator(Entity* entity);

  void SetDirection(int direction);
  int GetDirection() const;

  Animation::Clip* walkingAnimation;
  Animation::Clip* attackingAnimation;

  std::vector<Ref<Animation::Clip>> animations;
  Ref<Animation::Clip>& GetCurrentAnimation();

  float GetState() const;
  void SetState(float newState);

private:
  float state { 0 };
  int direction { 1 };
};