#pragma once

#include "Game/Subcomponents/Inventory/Inventory.h"

#include "Werwel/Texture.h"

class InterfaceRenderer {
public:
  InterfaceRenderer(const Inventory& inventory);

  void Render();

private:
  void RenderInventory();

  const Inventory& inventory;

  InterfaceRenderer(const InterfaceRenderer&) = delete;
  InterfaceRenderer& operator=(const InterfaceRenderer&) = delete;
};