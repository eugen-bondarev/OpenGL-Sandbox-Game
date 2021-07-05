#pragma once

#include "Component.h"

#include "ITransform.h"
#include "IUpdatable.h"

#include "Game/World/World.h"

#include "Entity.h"

#include "../Subcomponents/Inventory/Inventory.h"

class Player : public IUpdatable, public Component {
public:
  Player(Entity* entity, Ref<World> world);

  void Update() override;

  Icon GetCurrentItem() const;

  Inventory& GetInventory();

private:
  const Ref<World> world;

  Inventory inventory;
};