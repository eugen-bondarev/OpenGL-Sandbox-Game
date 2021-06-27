#include "InterfaceRenderer.h"

#include "Core/Window.h"

#include "imgui/imgui.h"

InterfaceRenderer::InterfaceRenderer(const Inventory& inventory, const Ref<Werwel::Texture> tileMap) : inventory { inventory }, tileMap { tileMap } {

}

void InterfaceRenderer::Render() {
  RenderInventory();
}

void InterfaceRenderer::RenderInventory() {
  static Vec2 buttonSize = Vec2(32.0f);
  static Vec2 padding = Vec2(4, 4);
  static Vec2 fullButtonSize = buttonSize + padding;
  static Vec2 amountOfButtons = Vec2(10, 1);
  static Vec2 barSize = (fullButtonSize + 16.0f) * amountOfButtons + Vec2(buttonSize.x * 1.5f + padding.x, 0);
  static Vec2 barPosition = Vec2(Window::GetSize().x / 2.0f - barSize.x / 2.0f, Window::GetSize().y - barSize.y - 20.0f);

  ImGui::SetNextWindowPos(ImVec2(barPosition.x + buttonSize.x / 2.0f, barPosition.y));
  ImGui::SetNextWindowSize(ImVec2(barSize.x, barSize.y + buttonSize.y / 2.0f));

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  windowFlags |= ImGuiWindowFlags_NoBackground;
  windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  windowFlags |= ImGuiWindowFlags_NoResize;

  ImGui::Begin("Inventory", nullptr, windowFlags);
  int posY = ImGui::GetCursorPosY();
  int posX = ImGui::GetCursorPosX() - padding.x;
    for (int i = 0; i < 10; i++) {
      ImGui::SetCursorPosY(posY);
      ImGui::SetCursorPosX(posX + (i + 1) * (buttonSize.x + padding.x / 2.0f + 16));
      int amountOfTilesX = 6.0f;
      int amountOfTilesY = 12.0f;
      int tileX = 2;
      int tileY = 3;
      ImVec2 uv0 = ImVec2(1.0f / amountOfTilesX * tileX, 1.0f / amountOfTilesY * tileY);
      ImVec2 uv1 = ImVec2(1.0f / amountOfTilesX * (tileX + 1), 1.0f / amountOfTilesY * (tileY + 1));
      ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y + 4), uv0, uv1);
    }
  ImGui::End();
}