#pragma once

#include "Game/Subcomponents/Inventory/Inventory.h"

#include "Werwel/Texture.h"

class InterfaceRenderer {
public:
  InterfaceRenderer(Inventory& inventory);
  void Render();

private:
  void RenderSideMenu();
  void RenderBottomBar(const std::string& windowID, Vec2 position, Vec2 amountOfButtons, int startingIndex = 0);

  Inventory& inventory;

  bool inventoryOpen { false };

  InterfaceRenderer(const InterfaceRenderer&) = delete;
  InterfaceRenderer& operator=(const InterfaceRenderer&) = delete;
};