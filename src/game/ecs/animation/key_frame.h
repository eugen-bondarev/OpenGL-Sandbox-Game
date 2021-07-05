#pragma once
			
struct KeyFrame {
  KeyFrame(Vec2 pos, float rot) : pos { pos }, rot { rot } { }
  Vec2 pos  { 0, 0 };
  float rot { 0 };

  void ApplyTo(Mat4& transform) const {    
    transform = Math::Translate(transform, Vec3(pos, 0.0f));
    transform = Math::Rotate(transform, Math::Radians(rot), Vec3(0, 0, 1));
  }
};