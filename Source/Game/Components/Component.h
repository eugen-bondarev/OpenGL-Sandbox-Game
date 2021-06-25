#pragma once

class Entity;

class Component {
public:
  Component(Ref<Entity> entity) : entity { entity } {

  }
  
  virtual ~Component() = default;

  Ref<Entity> entity;
};