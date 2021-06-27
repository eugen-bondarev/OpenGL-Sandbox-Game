#pragma once

#include "Component.h"

#include "ITransform.h"
#include "IUpdatable.h"

#include "../World.h"

#include "Entity.h"

#include "../Subcomponents/Inventory/Inventory.h"

class Player : public IUpdatable, public Component {
public:
  Player(Entity* entity, Ref<World> world) : Component(entity), world { world } {

  }

  void Update() override;

  const Inventory& GetInventory() const;

private:
  const Ref<World> world;

	int selectedItem = 0;

  Inventory inventory;
};