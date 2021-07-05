#include "iupdatable.h"

void IUpdatable::UpdateComponents() {
  for (auto& component : updatableComponents) {
    component->Update();
  }
}