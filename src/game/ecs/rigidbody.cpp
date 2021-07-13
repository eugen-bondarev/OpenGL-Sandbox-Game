#include "rigidbody.h"

#include "Linow/Linow.h"

#include "interface/console.h"

#include "sstream"

Rigidbody::Rigidbody(Entity *entity) : Component(entity)
{
}

Blocks::Representations::IndexAndPosition Rigidbody::GetBlockNearby(Vec2 fix, Vec2 offset) const
{
	Blocks::Representations::IndexAndPosition block;

	block.worldPosition = ToInt((entity->position + fix + offset * Map::BLOCK_SIZE) / Map::BLOCK_SIZE) * Map::BLOCK_SIZE - 8.0f;
	block.index = (block.worldPosition + 8.0f) / Map::BLOCK_SIZE;

	return block;
}

static constexpr bool RENDER_COLLIDERS = false;

void Rigidbody::CheckCollisions()
{
	// auto &blocks = map->GetBlocks();
	auto &blocks = Map::GetBlocks();

	State = RigidbodyState_Undefined;

	// onGround = false;
	// ceiling = false;
	// canMoveLeft = true;
	// canMoveRight = true;

	// - Map::VisibleChunks.x.start;
	// - Map::VisibleChunks.y.start;

	for (int i = 2; i < 4; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, 10.0f), Vec2(i, 0));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start].type != BlockType::Empty)
		{
			// entity->SetPositionY(block.worldPosition.y + Map::BLOCK_SIZE - 4.0f);

			if (velocity.y < 0)
			{
				// We don't want to test if we fell through a couple of blocks, if velocity is low.
				int amountOfBlocksToTest = std::max<float>(sqrt(abs(velocity.y)) - 25.0f, 0.0f);
				for (int j = amountOfBlocksToTest; j >= 1; j--)
				{
					if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start + j].type != BlockType::Empty)
					{
						// entity->SetPositionY(block.worldPosition.y + Map::BLOCK_SIZE * j + Map::BLOCK_SIZE - 4.0f);
						break;
					}
				}
			}

			// onGround = true;
			State |= RigidbodyState_OnGround;
			break;
		}
	}

	for (int i = 2; i < 4; i++)
	{
		Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-10.0f, -10.0f), Vec2(i, 6));

		if (RENDER_COLLIDERS)
			Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start - 1].type != BlockType::Empty)
		{
			// entity->SetPositionY(block.worldPosition.y - 5 * Map::BLOCK_SIZE);
			// ceiling = true;
			// State |= RigidbodyState_OnGround;
			break;
		}
	}

	for (int i = 1; i < 5; i++)
	{
		// Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f * (onGround ? 1 : -1)), Vec2(1, i));

		// if (RENDER_COLLIDERS)
		// 	Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		// if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start].type != BlockType::Empty)
		// {
		// 	// canMoveLeft = false;
		// 	break;
		// }
	}

	for (int i = 1; i < 5; i++)
	{
		// Blocks::Representations::IndexAndPosition block = GetBlockNearby(Vec2(-2.0f, 10.0f * (onGround ? 1 : -1)), Vec2(3, i));

		// if (RENDER_COLLIDERS)
		// 	Linow::AddQuad(block.worldPosition, block.worldPosition + Map::BLOCK_SIZE);

		// if (blocks[block.index.x - Map::VisibleChunks.x.start][block.index.y - Map::VisibleChunks.y.start].type != BlockType::Empty)
		// {
		// 	// canMoveRight = false;
		// 	break;
		// }
	}
}

// If something breaks - return to commit ccad6519c360441f689cbf9555e2dbaf31d122cc
void Rigidbody::CheckBottomCollisions()
{
	if (velocity.y > 0)
		return;

	State &= ~RigidbodyState_OnGround;

	bool pos_y_is_positive = entity->position.y >= 0.0f;

	static float shift = 4.0f;

	if (!pos_y_is_positive)
		entity->position.y -= shift;

	float factor = static_cast<int>(truncf(abs(entity->position.y) / 8.0f) * 1) % 2;

	Vec2 desired_pos = trunc((entity->position + Vec2(16.0f, factor ? 8.0f : 0.0f)) / Map::BLOCK_SIZE) * Map::BLOCK_SIZE;
	Vec2 block_index = (desired_pos - Vec2(Map::GetVisibleChunks().x.start, Map::GetVisibleChunks().y.start) * Map::ChunkSize * Map::BLOCK_SIZE) / Map::BLOCK_SIZE;

	if (!pos_y_is_positive)
		entity->position.y += shift;

	for (int i = 0; i < 2; i++)
	{
		if (!Map::InBounds(block_index.x + i, block_index.y))
			continue;

		if (Map::Blocks[block_index.x + i][block_index.y].type != BlockType::Empty)
		{			
			State |= RigidbodyState_OnGround;
			entity->SetPositionY(desired_pos.y + shift);
			break;
		}
	}
}

void Rigidbody::CheckCollisions1()
{
	Blocks_t &blocks = Map::GetBlocks();

	// ceiling = false;
	// canMoveLeft = true;
	// canMoveRight = true;

	CheckBottomCollisions();
}

void Rigidbody::Update()
{
	MW_PROFILER_SCOPE();

	State |= RigidbodyState_CanMoveLeft | RigidbodyState_CanMoveRight;

	if (!(State & RigidbodyState_OnGround))
	{
		entity->SetPosition(entity->position + velocity * Vec2(0, 1) * mw::Time::GetDelta());
		velocity += -Physics::g * mw::Time::GetDelta() * velocityFactor;

		if (State & RigidbodyState_Ceiling)
		{
			velocity.y = 0.0f;
			State &= ~RigidbodyState_Ceiling;
		}
	}
	else
	{
		velocity.y = 0.0f;
	}

	CheckCollisions1();
}

void Rigidbody::Jump()
{
	entity->SetPositionY(entity->position.y + Map::BLOCK_SIZE - 4.0f - 10.0f);
	velocity = Vec2(0.0f, 3.25f * velocityFactor);
	State &= ~RigidbodyState_OnGround;
}

RigidbodyState_ Rigidbody::GetState() const
{
	return State;
}