#pragma once

#include "component.h"

#include "itransform.h"
#include "iupdatable.h"

#include "game/world/map.h"

#include "entity.h"

class Rigidbody : public IUpdatable, public Component
{
public:
	inline static constexpr float velocityFactor = 150.0f;

	Blocks::Representations::IndexAndPosition GetBlockNearby(Vec2 fix, Vec2 offset = Vec2(0, 0)) const;

	Rigidbody(Entity *entity);

	void Update() override;
	void CheckCollisions();
	void Jump();

	void SetCeiling(bool value);
	void SetOnGround(bool value);
	void CanMoveLeft(bool value);
	void CanMoveRight(bool value);

	bool GetCeiling() const;
	bool GetOnGround() const;
	bool CanMoveLeft() const;
	bool CanMoveRight() const;

private:
	bool ceiling{false};
	bool onGround{false};
	bool canMoveLeft{true};
	bool canMoveRight{true};

	Vec2 velocity{0};
};