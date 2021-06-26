#pragma once

#include "Component.h"

#include "ITransform.h"
#include "IUpdatable.h"

#include "../Map.h"

#include "Entity.h"

class Player : public IUpdatable, public Component {
public:
  Player(Entity* entity) : Component(entity) {

  }

  void Update() override;
};