#pragma once

#include "Component.h"

#include "Animation/Animation.h"
#include "Animation/Humanoid/Walking.h"
#include "Animation/Humanoid/Attacking.h"

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