#include "Animator.h"

Animator::Animator(Entity* entity) : Component(entity) {
	animations.emplace_back(new Animation::Humanoid::Walking());
	animations.emplace_back(new Animation::Humanoid::Attacking());

	walkingAnimation = animations[0].get();
	attackingAnimation = animations[1].get();
}

void Animator::SetDirection(int direction) {
  this->direction = direction;
}

int Animator::GetDirection() const {
  return direction;
}

Ref<Animation::Clip>& Animator::GetCurrentAnimation() {
	for (int i = 0; i < animations.size(); i++) {
		if (animations[i]->InUse(state)) {
			return animations[i];
		}
	}

	return animations[0];
}

float Animator::GetState() const {
	return state;
}

void Animator::SetState(float newState) {
	state = newState;
}