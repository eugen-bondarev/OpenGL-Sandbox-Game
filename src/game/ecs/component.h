#pragma once

#include "iupdatable.h"

class Entity;

class Component {
public:
  Component(Entity* entity) : entity { entity } {
  }
  
  virtual ~Component() = default;

  Entity* entity;
};