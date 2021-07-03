#pragma once

#include "Component.h"
			
struct KeyFrame {
  KeyFrame(Vec2 pos, float rot) : pos { pos }, rot { rot } { }
  Vec2 pos  { 0, 0 };
  float rot { 0 };

  void ApplyTo(Mat4& transform) const {    
    transform = Math::Translate(transform, Vec3(pos, 0.0f));
    transform = Math::Rotate(transform, Math::Radians(rot), Vec3(0, 0, 1));
  }
};

struct Animation {
  std::vector<KeyFrame> keyFrames;
  float time { 0 };
};

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