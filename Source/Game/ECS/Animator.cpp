#include "Animator.h"

Animator::Animator(Entity* entity) : Component(entity) {	
	animation0.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 8, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 8, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 8, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 18 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 4, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	animation0.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);

	animation1.keyFrames.emplace_back(Vec2 { 24, 36 },  30);
	animation1.keyFrames.emplace_back(Vec2 { 26, 34 },  15);
	animation1.keyFrames.emplace_back(Vec2 { 28, 32 },  0);
	animation1.keyFrames.emplace_back(Vec2 { 30, 30 }, -25);
	animation1.keyFrames.emplace_back(Vec2 { 28, 28 }, -45);
	animation1.keyFrames.emplace_back(Vec2 { 26, 26 }, -60);
	animation1.keyFrames.emplace_back(Vec2 { 24, 24 }, -75);
}

// void Animator::SetFrame(float frame) {
//   this->frame = frame;
// }

// void Animator::SetAttackFrame(float frame) {
//   this->attackFrame = frame;
// }

void Animator::SetDirection(int direction) {
  this->direction = direction;
}

// float Animator::GetFrame() const {
//   return frame;
// }

// float Animator::GetAttackFrame() const {
//   return attackFrame;
// }

int Animator::GetDirection() const {
  return direction;
}

// float Animator::GetCurrentAnimationFrame() const {
  // return 
// }