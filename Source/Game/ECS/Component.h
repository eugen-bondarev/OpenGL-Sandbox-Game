#pragma once

#include "IUpdatable.h"

class Entity;

class Component {
public:
  inline static std::vector<IUpdatable*> updatableComponents;

  inline static void UpdateComponents() {
    for (auto& component : updatableComponents) {
      component->Update();
    }
  }

  Component(Entity* entity) : entity { entity } {

  }
  
  virtual ~Component() = default;

  Entity* entity;
};