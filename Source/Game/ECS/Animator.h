#pragma once

#include "Component.h"

class Entity;

class Animator : public Component {
public:
  Animator(Entity* entity) : Component(entity) {
  }

  void SetFrame(float frame) {
    this->frame = frame;
  }

  void SetDirection(int direction) {
    this->direction = direction;
  }

  float GetFrame() const {
    return frame;
  }

  int GetDirection() const {
    return direction;
  }

private:
  float frame { 0 };
  int direction { 1 };
};