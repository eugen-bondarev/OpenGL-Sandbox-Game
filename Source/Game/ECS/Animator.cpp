#include "Animator.h"

Animator::Animator(Entity* entity) : Component(entity) {	
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 8, 18 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 8, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 8, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 18 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 4, 18 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 4, 18 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 4, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 4, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);
	walkingAnimation.keyFrames.emplace_back(Vec2 { 6, 16 }, 0);

	attackingAnimation.keyFrames.emplace_back(Vec2 { 24, 36 },  30);
	attackingAnimation.keyFrames.emplace_back(Vec2 { 26, 34 },  15);
	attackingAnimation.keyFrames.emplace_back(Vec2 { 28, 32 },  0);
	attackingAnimation.keyFrames.emplace_back(Vec2 { 30, 30 }, -25);
	attackingAnimation.keyFrames.emplace_back(Vec2 { 28, 28 }, -45);
	attackingAnimation.keyFrames.emplace_back(Vec2 { 26, 26 }, -60);
	attackingAnimation.keyFrames.emplace_back(Vec2 { 24, 24 }, -75);
}

void Animator::SetDirection(int direction) {
  this->direction = direction;
}

int Animator::GetDirection() const {
  return direction;
}