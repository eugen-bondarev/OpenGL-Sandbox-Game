#pragma once

#include "component.h"

#include "itransform.h"
#include "iupdatable.h"

#include "game/world/map.h"

#include "entity.h"

enum RigidbodyState_
{
	RigidbodyState_Undefined 		= 0,
	RigidbodyState_OnGround 		= 1 << 0,
	RigidbodyState_CanMoveLeft 		= 1 << 1,
	RigidbodyState_CanMoveRight 	= 1 << 2,
	RigidbodyState_Ceiling 			= 1 << 3,
};
MW_ENUM_USE_FOR_FLAGS(RigidbodyState_)

class Rigidbody : public IUpdatable, public Component
{
public:
	inline static constexpr float velocityFactor = 150.0f;

	Blocks::Representations::IndexAndPosition GetBlockNearby(Vec2 fix, Vec2 offset = Vec2(0, 0)) const;

	Rigidbody(Entity *entity);

	void Update() override;
	void CheckCollisions();

	void CheckBottomCollisions();
	void CheckCollisions1();
	void Jump();

	RigidbodyState_ GetState() const;

private:
	RigidbodyState_ State;

	Vec2 velocity{0};
};