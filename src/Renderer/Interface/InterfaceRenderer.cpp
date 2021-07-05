#include "InterfaceRenderer.h"

#include "Core/Window.h"

#include "imgui/imgui.h"

#include "Renderer/World/Map/MapRenderer.h"

InterfaceRenderer::InterfaceRenderer(Inventory& inventory) : inventory { inventory } {

}

void InterfaceRenderer::Render() {
  RenderSideMenu();

  if (inventoryOpen) {
    RenderBottomBar("Inventory", Vec2(0, 100), Vec2(10, 6), 10);
  }

  RenderBottomBar("BottomBar", Vec2(0, 5), Vec2(10, 1));
}

void InterfaceRenderer::RenderSideMenu() {
  ImGui::SetNextWindowPos(ImVec2(20, Window::GetSize().y - 100 - 20));
  ImGui::SetNextWindowSize(ImVec2(100, 100));

  ImGui::Begin("Side menu");
    if (NF_KEY_PRESSED(NF_KEY_E) || ImGui::Button("Inventory (E)")) {
      inventoryOpen = !inventoryOpen;
    }
  ImGui::End();
}

void InterfaceRenderer::RenderBottomBar(const std::string& windowID, Vec2 position, Vec2 amountOfButtons, int startingIndex) {  
  static Vec2 buttonSize = Vec2(32.0f);
  static Vec2 padding = Vec2(2);
  static Vec2 innerPadding = Vec2(4, 3);
  static Vec2 fullButtonSize = buttonSize + (innerPadding + padding) * 2.0f;
  Vec2 barSize = fullButtonSize * amountOfButtons + Vec2(0, 20 /* title bar */) + (Vec2(ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y) - ImGui::GetStyle().WindowBorderSize) * 2.0f;
  Vec2 barPosition = Vec2((Window::GetSize().x - barSize.x) / 2.0f, Window::GetSize().y - barSize.y - position.y);

  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  // windowFlags |= ImGuiWindowFlags_NoBackground;
  // windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  windowFlags |= ImGuiWindowFlags_NoResize;
  windowFlags |= ImGuiWindowFlags_NoScrollbar;
  windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

  // ImGui::SetNextWindowPos(ImVec2(Window::GetPosition().x + barPosition.x, barPosition.y));
  ImGui::SetNextWindowPos(ImVec2(barPosition.x, barPosition.y));
  ImGui::SetNextWindowSize(ImVec2(barSize.x, barSize.y));

  ImGui::Begin(windowID.c_str(), nullptr, windowFlags);

  int startPosX = ImGui::GetCursorPosX();
  int startPosY = ImGui::GetCursorPosY();

  int index = startingIndex;

  for (int i = 0; i < amountOfButtons.x; i++) {
    for (int j = 0; j < amountOfButtons.y; j++) {
      Vec2 pos = Vec2(startPosX + i * (buttonSize.x + (innerPadding.x + padding.x) * 2), startPosY + j * (buttonSize.y + (innerPadding.y + padding.y) * 2));

      ImGui::SetCursorPos(ImVec2(pos));

      if (!inventory.items[index]->IsEmpty() && inventory.items[index]->GetCurrentAmount()) {
        Icon& icon = inventory.items[index]->GetIcon();
        TileMap* tileMap = icon.first;
        Vec2 tile = icon.second;      

        ImVec2 uv0 = ImVec2(Vec2(1.0f / tileMap->GetAmountOfTiles() * tile));
        ImVec2 uv1 = ImVec2(Vec2(1.0f / tileMap->GetAmountOfTiles() * (tile + 1.0f)));

        ImGui::PushID(index);
        if (inventory.selectedItem == index) {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
            ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1, -1);
          ImGui::PopStyleColor();
        } else {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.4f));
            ImGui::ImageButton((void*)(intptr_t) tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1);
          ImGui::PopStyleColor();
        }
        ImGui::PopID();

        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
          ImGui::SetDragDropPayload("DND_DEMO_CELL", &index, sizeof(int));
          ImGui::EndDragDropSource();
        }

        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL")) {
            int ind = *static_cast<const int*>(payload->Data);
            inventory.Swap(ind, index);
          }
          ImGui::EndDragDropTarget();
        }

        if (inventory.items[index]->GetStack() != 1) {
          ImGui::SetCursorPos(ImVec2(pos));
          ImGui::Text(std::to_string(inventory.items[index]->GetCurrentAmount()).c_str());
        }

      } else {
        if (inventory.selectedItem == index) {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.8f, 0.8f, 0.8f));
        }
        else {
          ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.4f, 0.4f, 0.4f));
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

        if (ImGui::BeginDragDropTarget()) {
          if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL")) {
            int ind = *static_cast<const int*>(payload->Data);
            inventory.Swap(ind, index);
          }
          ImGui::EndDragDropTarget();
        }

        ImGui::PopStyleColor();
      }
      index += 1;
    }
  }

  ImGui::End();
}