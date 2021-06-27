#include "InterfaceRenderer.h"

#include "Core/Window.h"

#include "imgui/imgui.h"

#include "../MapRenderer.h"

InterfaceRenderer::InterfaceRenderer(const Inventory& inventory, const Ref<Werwel::Texture> tileMap) : inventory { inventory }, tileMap { tileMap } {

}

void InterfaceRenderer::Render() {
  RenderInventory();
}

void InterfaceRenderer::RenderInventory() {
  static Vec2 buttonSize = Vec2(32.0f);
  static Vec2 padding = Vec2(2);
  static Vec2 innerPadding = Vec2(4);
  static Vec2 fullButtonSize = buttonSize + padding * 2.0f + innerPadding * 2.0f;
  static Vec2 amountOfButtons = Vec2(10, 1);
  static Vec2 barSize = Vec2(Window::GetSize().x, fullButtonSize.y + (padding.y + innerPadding.y) / 2.0f);
  static int bottomMargin = 5.0f;
  static Vec2 barPosition = Vec2(0, Window::GetSize().y - barSize.y - bottomMargin);

  ImGui::SetNextWindowPos(ImVec2(Window::GetPosition().x + barPosition.x, barPosition.y));
  ImGui::SetNextWindowSize(ImVec2(barSize.x, barSize.y));

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  windowFlags |= ImGuiWindowFlags_NoBackground;
  windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  windowFlags |= ImGuiWindowFlags_NoResize;
  windowFlags |= ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

  ImGui::Begin("Inventory", nullptr, windowFlags);
  
  int posY = ImGui::GetCursorPosY();
  int posX = ImGui::GetCursorPosX() - padding.x * 2;

  for (int i = -amountOfButtons.x / 2.0f; i < amountOfButtons.x / 2.0f; i++) {
    int amountOfTilesX = tileMap->GetSize().x * 2.0f / 16.0f;
    int amountOfTilesY = tileMap->GetSize().y * 2.0f / 16.0f;

    int index = i + amountOfButtons.x / 2.0f;

    ImGui::SetCursorPosY(posY);
    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f + posX + i * (fullButtonSize.x - padding.x));

    if (!inventory.cells[index].IsEmpty() && inventory.cells[index].quantity) {
      int tileX, tileY;

      if (inventory.cells[index].type == ItemType::Block) {
        tileX = blocksTextureDictionary[inventory.cells[index].data.blockType].x + 1;
        tileY = blocksTextureDictionary[inventory.cells[index].data.blockType].y + 2;
      } else {
        tileX = blocksTextureDictionary[inventory.cells[index].data.wallType].x + 4;
        tileY = blocksTextureDictionary[inventory.cells[index].data.wallType].y + 2;
      }

      ImVec2 uv0 = ImVec2(1.0f / amountOfTilesX * tileX, 1.0f / amountOfTilesY * tileY);
      ImVec2 uv1 = ImVec2(1.0f / amountOfTilesX * (tileX + 1), 1.0f / amountOfTilesY * (tileY + 1));

      ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1);

      ImGui::SetCursorPosY(posY);
      ImGui::SetCursorPosX(Window::GetSize().x / 2.0f + posX + i * (fullButtonSize.x - padding.x));
      ImGui::Text(std::to_string(inventory.cells[index].quantity).c_str());

    } else {
      ImGui::ImageButton(
        (void*)(intptr_t) tileMap->GetHandle(), 
        ImVec2(buttonSize.x, buttonSize.y), 
        ImVec2(0, 0), 
        ImVec2(1, 1), 
        -1,
        ImVec4(0, 0, 0, 0),
        ImVec4(0, 0, 0, 0)
      );
    }
  }

  ImGui::End();
}