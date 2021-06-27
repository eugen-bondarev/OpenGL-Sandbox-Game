#pragma once

#include "Game/Subcomponents/Inventory/Inventory.h"

#include "Werwel/Texture.h"

class InterfaceRenderer {
public:
  InterfaceRenderer(const Inventory& inventory, const Ref<Werwel::Texture> tileMap);

  void Render();

private:
  void RenderInventory();

  const Inventory& inventory;
  const Ref<Werwel::Texture> tileMap;

  InterfaceRenderer(const InterfaceRenderer&) = delete;
  InterfaceRenderer& operator=(const InterfaceRenderer&) = delete;
};