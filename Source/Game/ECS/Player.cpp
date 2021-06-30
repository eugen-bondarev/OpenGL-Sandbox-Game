#include "Player.h"

#include "Core/Input/Input.h"
#include "Core/Time.h"

#include "Entity.h"
#include "Rigidbody.h"
#include "Animator.h"

#include "../Items/Empty.h"

Player::Player(Entity* entity, Ref<World> world) : Component(entity), world { world } {

}

void Player::Update() {
	if (Input::KeyPressed(Input::Key::Space)) {
		if (entity->rigidbody->GetOnGround()) {
			entity->rigidbody->Jump();
		}
	}

	static float defaultSpeed = 150.0f;

	if (Input::KeyDown(Input::Key::A) && entity->rigidbody->CanMoveLeft()) {
		entity->SetPosition(entity->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * defaultSpeed);
		entity->animator->SetFrame(entity->animator->GetFrame() - 0.2f * Time::GetDelta() * defaultSpeed);
		entity->animator->SetDirection(-1);
	} else if (Input::KeyDown(Input::Key::D) && entity->rigidbody->CanMoveRight()) {
		entity->SetPosition(entity->GetPosition() + Vec2(1, 0) * Time::GetDelta() * defaultSpeed);
		entity->animator->SetFrame(entity->animator->GetFrame() + 0.2f * Time::GetDelta() * defaultSpeed);
		entity->animator->SetDirection(1);
	} else {
		float frame = entity->animator->GetDirection() == 1 ? 0 : -1;
		if (entity->animator->GetFrame() != frame) {
			entity->animator->SetFrame(frame);
			world->GetMap()->blocksUpdated = true;
		}
	}

	if (Input::GetMouseWheelState() == MouseWheelState::Up) {
		inventory.selectedItem -= 1;
		if (inventory.selectedItem < 0) {
			inventory.selectedItem = 9;
		}
	}

	if (Input::GetMouseWheelState() == MouseWheelState::Down) {
		inventory.selectedItem += 1;
		if (inventory.selectedItem > 9) {
			inventory.selectedItem = 0;
		}
	}

	if (Input::MouseButtonDown(Input::Button::Left)) {
		inventory.items[inventory.selectedItem]->Use(GameState(world.get(), this));

		if (!inventory.items[inventory.selectedItem]->GetCurrentAmount()) {
			inventory.items[inventory.selectedItem].reset(new Empty());
		}
	}
}

Inventory& Player::GetInventory() {
	return inventory;
}