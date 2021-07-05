#pragma once

#include "maths/maths.h"

#include "game/ecs/animator.h"
#include "game/ecs/rigidbody.h"
#include "game/ecs/player.h"
#include "game/ecs/entity.h"

#include "game/world/world.h"

class Character : public Entity
{
public:
	Character(const Ref<World> &world);
	void CollectLights(std::vector<Vec2> &lights) const;
	void Land();

private:
	const Ref<World> &world;

	void CalculateTransform() override
	{
		transform = Math::Translate(Mat4(1), Vec3(position, 0.0));
	}

	Character(const Character &) = delete;
	Character &operator=(const Character &) = delete;
};