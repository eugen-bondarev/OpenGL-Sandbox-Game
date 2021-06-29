#include "InterfaceRenderer.h"

#include "Core/Window.h"

#include "imgui/imgui.h"

#include "Renderer/World/Map/MapRenderer.h"

InterfaceRenderer::InterfaceRenderer(const Inventory& inventory) : inventory { inventory } {

}

void InterfaceRenderer::Render() {
  RenderInventory();
}

void InterfaceRenderer::RenderInventory() {
  // Ref<BlocksTileMap> tileMap = TextureAtlas::Get<BlocksTileMap>(TextureAtlasType::Map);

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
    int index = i + amountOfButtons.x / 2.0f;

    ImGui::SetCursorPosY(posY);
    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f + posX + i * (fullButtonSize.x - padding.x));

    if (!inventory.items[index]->IsEmpty() && inventory.items[index]->GetCurrentAmount()) {
      Icon& icon = inventory.items[index]->GetIcon();
      TileMap* tileMap = icon.first;
      Vec2 tile = icon.second;      

      ImVec2 uv0 = ImVec2(Vec2(1.0f / tileMap->GetAmountOfTiles() * tile));
      ImVec2 uv1 = ImVec2(Vec2(1.0f / tileMap->GetAmountOfTiles() * (tile + 1.0f)));

      if (inventory.selectedItem == index) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.8, 0.8, 1));
          ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1, -1);
        ImGui::PopStyleColor();
      } else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.4, 0.4, 0.4));
          ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1);
        ImGui::PopStyleColor();
      }

      if (inventory.items[index]->GetStack() != 1) {
        ImGui::SetCursorPosY(posY);
        ImGui::SetCursorPosX(Window::GetSize().x / 2.0f + posX + i * (fullButtonSize.x - padding.x));
        ImGui::Text(std::to_string(inventory.items[index]->GetCurrentAmount()).c_str());
      }

    } else {
      if (inventory.selectedItem == index) {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.8, 0.8, 1));
      }
      else {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4, 0.4, 0.4, 0.4));
      }

      ImGui::ImageButton(
        (void*)(intptr_t) 0, 
        ImVec2(buttonSize.x, buttonSize.y), 
        ImVec2(0, 0), 
        ImVec2(1, 1), 
        -1,
        ImVec4(0, 0, 0, 0),
        ImVec4(0, 0, 0, 0)
      );
      ImGui::PopStyleColor();
    }
  }

  ImGui::End();
}