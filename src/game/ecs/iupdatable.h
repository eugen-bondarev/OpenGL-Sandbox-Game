#pragma once

class IUpdatable {
public:
  inline static std::vector<IUpdatable*> updatableComponents;

  static void UpdateComponents();

  virtual void Update() = 0;
};