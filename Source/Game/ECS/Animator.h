#pragma once

#include "Component.h"

class Entity;

class Animator : public Component {
public:
  Animator(Entity* entity);

  void SetFrame(float frame);
  void SetAttackFrame(float frame);
  void SetDirection(int direction);

  float GetFrame() const;
  float GetAttackFrame() const;
  int GetDirection() const;

  int state { 0 };

private:
  float frame { 0 };
  int direction { 1 };

  float attackFrame { 0 };
};