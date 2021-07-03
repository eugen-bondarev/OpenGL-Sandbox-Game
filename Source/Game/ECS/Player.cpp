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
		// float frame = entity->animator->GetDirection() == 1 ? 0 : -1;
		// if (entity->animator->GetFrame() != frame) {
		// 	entity->animator->SetFrame(frame);
		// 	world->GetMap()->blocksUpdated = true;
		// }
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

		entity->animator->state = 1.0f;
	}

	// entity->animator->state = 0.0f;

	// if (NF_KEY_PRESSED(NF_KEY_0)) { entity->animator->SetFrame(0); entity->animator->SetAttackFrame(0); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_1)) { entity->animator->SetFrame(1); entity->animator->SetAttackFrame(1); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_2)) { entity->animator->SetFrame(2); entity->animator->SetAttackFrame(2); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_3)) { entity->animator->SetFrame(3); entity->animator->SetAttackFrame(3); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_4)) { entity->animator->SetFrame(4); entity->animator->SetAttackFrame(4); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_5)) { entity->animator->SetFrame(5); entity->animator->SetAttackFrame(5); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_6)) { entity->animator->SetFrame(6); entity->animator->SetAttackFrame(6); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_7)) { entity->animator->SetFrame(7); entity->animator->SetAttackFrame(7); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_ARROW_LEFT)) { entity->animator->SetFrame(entity->animator->GetFrame() - 1); entity->animator->SetAttackFrame(entity->animator->GetAttackFrame() - 1); world->GetMap()->blocksUpdated = true; }
	// if (NF_KEY_PRESSED(NF_KEY_ARROW_RIGHT)) { entity->animator->SetFrame(entity->animator->GetFrame() + 1); entity->animator->SetAttackFrame(entity->animator->GetAttackFrame() + 1); world->GetMap()->blocksUpdated = true; }

	if (entity->animator->state != 0.0f) {
		entity->animator->SetAttackFrame(entity->animator->GetAttackFrame() + 0.2f * Time::GetDelta() * defaultSpeed);
		if (entity->animator->GetAttackFrame() > 7.0f) {
			entity->animator->SetAttackFrame(0.0f);
			entity->animator->state = 0.0f;
		}
		world->GetMap()->blocksUpdated = true;
	}
}

Inventory& Player::GetInventory() {
	return inventory;
}

Icon Player::GetCurrentItem() const {
	return inventory.items[inventory.selectedItem]->GetIcon();
}