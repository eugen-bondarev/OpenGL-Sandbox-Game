#pragma once

#include "Component.h"

class Entity;

class Animator : public Component {
public:
  Animator(Entity* entity);

  void SetFrame(float frame);
  void SetDirection(int direction);

  float GetFrame() const;
  int GetDirection() const;

private:
  float frame { 0 };
  int direction { 1 };
};