#pragma once

#include "component.h"

#include "itransform.h"
#include "iupdatable.h"

#include "game/world/world.h"

#include "entity.h"

#include "../subcomponents/inventory/inventory.h"

class Player : public IUpdatable, public Component
{
public:
	Player(Entity *entity, Ref<World> world);

	void Update() override;

	Icon GetCurrentItem() const;

	Inventory &GetInventory();

private:
	const Ref<World> world;

	Inventory inventory;
};