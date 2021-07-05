#include "player.h"

#include "core/input/input.h"
#include "core/time.h"

#include "entity.h"
#include "rigidbody.h"
#include "animator.h"

#include "../items/empty.h"

Player::Player(Entity *entity, Ref<World> world) : Component(entity), world{world}
{
}

void Player::Update()
{
	if (Input::KeyPressed(Input::Key::Space))
	{
		if (entity->rigidbody->GetOnGround())
		{
			entity->rigidbody->Jump();
		}
	}

	static float defaultSpeed = 150.0f;

	if (entity->animator->walkingAnimation->GetTime() > 13)
	{
		entity->animator->walkingAnimation->SetTime(0);
	}

	if (entity->animator->walkingAnimation->GetTime() < 0)
	{
		entity->animator->walkingAnimation->SetTime(13);
	}

	if (Input::KeyDown(Input::Key::A) && entity->rigidbody->CanMoveLeft())
	{
		entity->SetPosition(entity->GetPosition() + Vec2(-1, 0) * Time::GetDelta() * defaultSpeed);
		entity->animator->walkingAnimation->SetTime(entity->animator->walkingAnimation->GetTime() - 0.2f * Time::GetDelta() * defaultSpeed);
		entity->animator->SetDirection(-1);
	}
	else if (Input::KeyDown(Input::Key::D) && entity->rigidbody->CanMoveRight())
	{
		entity->SetPosition(entity->GetPosition() + Vec2(1, 0) * Time::GetDelta() * defaultSpeed);
		entity->animator->walkingAnimation->SetTime(entity->animator->walkingAnimation->GetTime() + 0.2f * Time::GetDelta() * defaultSpeed);
		entity->animator->SetDirection(1);
	}
	else
	{
		float frame = entity->animator->GetDirection() == 1 ? 0 : -1;
		if (entity->animator->walkingAnimation->GetTime() != frame)
		{
			entity->animator->walkingAnimation->SetTime(0.0f);
			world->GetMap()->blocksUpdated = true;
		}
	}

	if (Input::GetMouseWheelState() == MouseWheelState::Up)
	{
		inventory.selectedItem -= 1;
		if (inventory.selectedItem < 0)
		{
			inventory.selectedItem = 9;
		}
		world->GetMap()->blocksUpdated = true;
	}

	if (Input::GetMouseWheelState() == MouseWheelState::Down)
	{
		inventory.selectedItem += 1;
		if (inventory.selectedItem > 9)
		{
			inventory.selectedItem = 0;
		}
		world->GetMap()->blocksUpdated = true;
	}

	if (Input::MouseButtonDown(Input::Button::Left))
	{
		inventory.items[inventory.selectedItem]->Use(GameState(world.get(), this));

		if (!inventory.items[inventory.selectedItem]->GetCurrentAmount())
		{
			inventory.items[inventory.selectedItem].reset(new Empty());
		}

		entity->animator->SetState(1.0f);
	}

	if (entity->animator->GetState() != 0.0f)
	{
		entity->animator->attackingAnimation->SetTime(entity->animator->attackingAnimation->GetTime() + 0.2f * Time::GetDelta() * defaultSpeed);
		if (entity->animator->attackingAnimation->GetTime() > 7.0f)
		{
			entity->animator->attackingAnimation->SetTime(0.0f);
			entity->animator->SetState(0.0f);
		}
		world->GetMap()->blocksUpdated = true;
	}
}

Inventory &Player::GetInventory()
{
	return inventory;
}

Icon Player::GetCurrentItem() const
{
	return inventory.items[inventory.selectedItem]->GetIcon();
}