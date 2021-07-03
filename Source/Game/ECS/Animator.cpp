#include "Animator.h"

Animator::Animator(Entity* entity) : Component(entity) {
}

void Animator::SetFrame(float frame) {
  this->frame = frame;
}

void Animator::SetAttackFrame(float frame) {
  this->attackFrame = frame;
}

void Animator::SetDirection(int direction) {
  this->direction = direction;
}

float Animator::GetFrame() const {
  return frame;
}

float Animator::GetAttackFrame() const {
  return attackFrame;
}

int Animator::GetDirection() const {
  return direction;
}