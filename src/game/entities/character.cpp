#include "character.h"

Character::Character(const Ref<World> &world) : world{world}
{
	AddComponent<Animator>();
	AddComponent<Rigidbody>(world->GetMap());
	AddComponent<Player>(world);

	SetPosition(world->GetCamera()->GetPosition());
	Land();
}

void Character::CollectLights(std::vector<Vec2> &lights) const
{
	NF_PROFILER_SCOPE();

	const auto &blocks = world->GetMap()->GetBlocks();
	const auto &walls = world->GetMap()->GetWalls();

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			auto block = rigidbody->GetBlockNearby(Vec2(), Vec2(1 + x, 2 + y * 2));
			if (blocks[block.index.x][block.index.y].type == BlockType::Empty && walls[block.index.x][block.index.y].type == WallType::Empty)
			{
				lights.push_back(block.worldPosition);
			}
		}
	}
}

void Character::Land()
{
	rigidbody->Update();

	bool posChanged{false};

	while (rigidbody->GetCeiling())
	{
		SetPositionY(GetPosition().y + 16.0f);
		rigidbody->Update();
		posChanged = true;
	}

	if (posChanged)
	{
		SetPositionY(GetPosition().y + 128.0f);
		rigidbody->Update();
	}

	while (!rigidbody->GetOnGround())
	{
		rigidbody->Update();
	}
}