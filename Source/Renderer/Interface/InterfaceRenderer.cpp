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
  static Vec2 padding = Vec2(2, 2);
  static Vec2 innerPadding = Vec2(6);
  static Vec2 fullButtonSize = buttonSize + padding * 2.0f + innerPadding * 2.0f;
  static Vec2 amountOfButtons = Vec2(10, 1);
  static Vec2 barSize = fullButtonSize * amountOfButtons;
  static Vec2 barPosition = Vec2(Window::GetSize().x / 2.0f - barSize.x / 2.0f, Window::GetSize().y - barSize.y - 20.0f);

  ImGui::SetNextWindowPos(ImVec2(Window::GetPosition().x + barPosition.x, Window::GetPosition().y + barPosition.y));
  ImGui::SetNextWindowSize(ImVec2(barSize.x, barSize.y + buttonSize.y / 2.0f));

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  windowFlags |= ImGuiWindowFlags_NoBackground;
  windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  windowFlags |= ImGuiWindowFlags_NoResize;

  ImGui::Begin("Inventory", nullptr, windowFlags);
  int posY = ImGui::GetCursorPosY();
  int posX = ImGui::GetCursorPosX() - padding.x * 2;

    for (int i = 0; i < 10; i++) {
      ImGui::SetCursorPosY(posY);
      ImGui::SetCursorPosX(posX + (i) * (fullButtonSize.x - 2));
      int amountOfTilesX = tileMap->GetSize().x * 2.0f / 16.0f;
      int amountOfTilesY = tileMap->GetSize().y * 2.0f / 16.0f;
      int tileX = 2;
      int tileY = 3;
      ImVec2 uv0 = ImVec2(1.0f / amountOfTilesX * tileX, 1.0f / amountOfTilesY * tileY);
      ImVec2 uv1 = ImVec2(1.0f / amountOfTilesX * (tileX + 1), 1.0f / amountOfTilesY * (tileY + 1));
      ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1, -1);
    }
  ImGui::End();
}