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
		float frame = entity->animator->GetDirection() == 1 ? 0 : -1;
		if (entity->animator->GetFrame() != frame) {
			entity->animator->SetFrame(frame);
			world->GetMap()->blocksUpdated = true;
		}
	}

	if (Input::KeyPressed(Key::Num1)) {
		selectedItem = 0;
	}

	if (Input::KeyPressed(Key::Num2)) {
		selectedItem = 1;
	}

	if (Input::MouseButtonDown(Button::Left)) {
		Map::BlockSettingData settingBlock;
			
		if (inventory.cells[selectedItem].type == ItemType::Block) {
			settingBlock = world->GetMap()->PlaceBlock(world->GetCamera()->GetPosition(), BlockType::Empty);
		} else if (inventory.cells[selectedItem].type == ItemType::Wall) {
			settingBlock = world->GetMap()->PlaceWall(world->GetCamera()->GetPosition(), WallType::Empty);
		}

		if (settingBlock.IsSet()) {
			inventory.cells[selectedItem].quantity += 1;
			inventory.cells[selectedItem].data.blockType = settingBlock.blockType;

			// inventory.cells[selectedItem].type = ItemType::Block;
		}
	}

	if (Input::MouseButtonDown(Button::Right)) {
		if (!inventory.cells[selectedItem].IsEmpty() && inventory.cells[selectedItem].quantity) {
			Map::BlockSettingData settingBlock;

			if (inventory.cells[selectedItem].type == ItemType::Block) {
				settingBlock = world->GetMap()->PlaceBlock(world->GetCamera()->GetPosition(), inventory.cells[selectedItem].data.blockType);
			} else if (inventory.cells[selectedItem].type == ItemType::Wall) {
				settingBlock = world->GetMap()->PlaceWall(world->GetCamera()->GetPosition(), inventory.cells[selectedItem].data.wallType);
			}

			if (settingBlock.IsSet()) {
				inventory.cells[selectedItem].quantity -= 1;
			}
		}
	}
}

const Inventory& Player::GetInventory() const {
	return inventory;
}