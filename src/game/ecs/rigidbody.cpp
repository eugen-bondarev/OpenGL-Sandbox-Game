#include "rigidbody.h"

#include "Linow/Linow.h"

Rigidbody::Rigidbody(Entity *entity, const Ref<Map> map) : Component(entity), map{map}
{
}

Blocks::Representations::IndexAndPosition Rigidbody::GetBlockNearby(Vec2 fix, Vec2 offset) const
{
	Blocks::Representations::IndexAndPosition block;
	block.worldPosition = ToInt((entity->position + fix + offset * map->GetBlockSize()) / map->GetBlockSize()) * map->GetBlockSize() - 8.0f;
	block.index = (block.worldPosition + 8.0f) / map->GetBlockSize();
	return block;
}

static constexpr bool RENDER_COLLIDERS = false;

void Rigidbody::CheckCollisions()
{
	auto &blocks = map->GetBlocks();

	onGround = false;
	ceiling = false;
	canMoveLeft = true;
	canMoveRight = true;

	for (int i = 2; i < 4; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, 10.0f), Vec2(i, 0));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

		if (blocks[block.index.x][block.index.y].type != BlockType::Empty)
		{
			entity->SetPositionY(block.worldPosition.y + map->GetBlockSize() - 4.0f);

			if (velocity.y < 0)
			{
				// We don't want to test if we fell through a couple of blocks, if velocity is low.
				int amountOfBlocksToTest = std::max<float>(sqrt(abs(velocity.y)) - 25.0f, 0.0f);
				for (int j = amountOfBlocksToTest; j >= 1; j--)
				{
					if (blocks[block.index.x][block.index.y + j].type != BlockType::Empty)
					{
						entity->SetPositionY(block.worldPosition.y + map->GetBlockSize() * j + map->GetBlockSize() - 4.0f);
						break;
					}
				}
			}

			onGround = true;
			break;
		}
	}

	for (int i = 2; i < 4; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, -10.0f), Vec2(i, 6));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

		if (blocks[block.index.x][block.index.y - 1].type != BlockType::Empty)
		{
			entity->SetPositionY(block.worldPosition.y - 5 * map->GetBlockSize());
			ceiling = true;
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f * (onGround ? 1 : -1)), Vec2(1, i));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

		if (blocks[block.index.x][block.index.y].type != BlockType::Empty)
		{
			canMoveLeft = false;
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f * (onGround ? 1 : -1)), Vec2(3, i));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + map->GetBlockSize());

		if (blocks[block.index.x][block.index.y].type != BlockType::Empty)
		{
			canMoveRight = false;
			break;
		}
	}
}

void Rigidbody::Update()
{
	MW_PROFILER_SCOPE();

	if (!onGround)
	{
		entity->SetPosition(entity->position + velocity * Vec2(0, 1) * mw::Time::GetDelta());
		velocity += -Physics::g * mw::Time::GetDelta() * velocityFactor;

		if (ceiling)
		{
			velocity.y = 0.0f;
			ceiling = false;
		}
	}
	else
	{
		velocity.y = 0.0f;
	}

	CheckCollisions();
}

void Rigidbody::Jump()
{
	entity->SetPositionY(entity->position.y + map->GetBlockSize() - 4.0f - 10.0f);
	velocity = Vec2(0.0f, 3.25f * velocityFactor);
	onGround = false;
}

void Rigidbody::SetCeiling(bool value) { ceiling = value; }
void Rigidbody::SetOnGround(bool value) { onGround = value; }
void Rigidbody::CanMoveLeft(bool value) { canMoveLeft = value; }
void Rigidbody::CanMoveRight(bool value) { canMoveRight = value; }

bool Rigidbody::GetCeiling() const { return ceiling; }
bool Rigidbody::GetOnGround() const { return onGround; }
bool Rigidbody::CanMoveLeft() const { return canMoveLeft; }
bool Rigidbody::CanMoveRight() const { return canMoveRight; }