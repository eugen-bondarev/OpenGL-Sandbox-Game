#include "Player.h"

#include "Core/Input/Input.h"
#include "Core/Time.h"

#include "Entity.h"
#include "Rigidbody.h"
#include "Animator.h"

void Player::Update() {
	if (Input::KeyPressed(Key::Space)) {
		if (entity->rigidbody->GetOnGround()) {
			entity->rigidbody->Jump();
		}
	}

	static float defaultSpeed = 150.0f;

	if (Input::KeyDown(Key::A) && entity->rigidbody->CanMoveLeft()) {
		entity->SetPosition(entity->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * defaultSpeed);
		entity->animator->SetFrame(entity->animator->GetFrame() - 0.2f * Time::GetDelta() * defaultSpeed);
		entity->animator->SetDirection(-1);
	} else if (Input::KeyDown(Key::D) && entity->rigidbody->CanMoveRight()) {
		entity->SetPosition(entity->GetPosition() + Vec2(1, 0) * Time::GetDelta() * defaultSpeed);
		entity->animator->SetFrame(entity->animator->GetFrame() + 0.2f * Time::GetDelta() * defaultSpeed);
		entity->animator->SetDirection(1);
	} else {
		entity->animator->SetFrame(entity->animator->GetDirection() == 1 ? 0 : -1);
	}
}