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

	static int selectedItem = 0;

	if (Input::MouseButtonDown(Button::Left)) {
		Map::BlockSettingData settingBlock = world->GetMap()->PlaceBlock(world->GetCamera()->GetPosition(), BlockType::Empty);

		if (settingBlock.IsSet()) {
			inventory.cells[selectedItem].quantity += 1;
			inventory.cells[selectedItem].data.blockType = settingBlock.blockType;
			inventory.cells[selectedItem].type = ItemType::Block;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		if (!inventory.cells[selectedItem].IsEmpty() && inventory.cells[selectedItem].quantity) {
			Map::BlockSettingData settingBlock = world->GetMap()->PlaceBlock(world->GetCamera()->GetPosition(), inventory.cells[selectedItem].data.blockType);

			if (settingBlock.IsSet()) {
				inventory.cells[selectedItem].quantity -= 1;
			}
		}
	}
}

const Inventory& Player::GetInventory() const {
	return inventory;
}