#include "MainMenu.h"

#include "Core/Window.h"
#include "Core/Gui.h"

#include "imgui/imgui.h"

MainMenu::MainMenu(Ref<Game>& game) : game { game } {

}

void MainMenu::Settings() {
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    ImGui::PushFont(Gui::titleFont);
      ImGui::Text("Settings");
    ImGui::PopFont();

  static WindowSettings windowSettings = {};

  static Vec2i s = { Window::GetSize().x, Window::GetSize().y };
  
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Recreate the window")) {
      Window::Recreate(windowSettings);
    }
  
  ImGui::PushItemWidth(150);
    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
      if (ImGui::InputInt("Width", &s.x)) { windowSettings.size = s; }
    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
      if (ImGui::InputInt("Height", &s.y)) { windowSettings.size = s; }

    const char* items[] = { "Fullscreen", "Borderless", "Windowed" };
    static int item_current = static_cast<int>(windowSettings.mode);

    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
      if (ImGui::Combo("Window mode", &item_current, items, 3)) {
        windowSettings.mode = static_cast<WindowMode>(item_current);
      }

    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
      ImGui::Checkbox("Maximize window", &windowSettings.maximize);

    ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
      ImGui::Checkbox("vSync", &windowSettings.vSync);
  ImGui::PopItemWidth();

  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Back")) {
      location = MenuLocation::Main;
    }
}

void MainMenu::Main() {
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    ImGui::PushFont(Gui::titleFont);
      ImGui::Text("Naturaforge");
    ImGui::PopFont();

  static int seed = 669;

  ImGui::SetNextItemWidth(100);
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    ImGui::InputInt("Seed", &seed);

  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Start game")) {
      game = CreatePtr<Game>(seed);
    }
  
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Settings")) {
      location = MenuLocation::Settings;
    }

  static bool ex = false;
  
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Exit")) {
      ex = true;
    }
  
  if (ex) {
    ImGui::SetNextWindowSize(ImVec2(300, 100));
    ImGui::OpenPopup("Are you sure?");
    if (ImGui::BeginPopupModal("Are you sure?", NULL)) {
      if (ImGui::Button("Yes")) {
        Window::Close();
      }

      ImGui::SameLine();
      if (ImGui::Button("No")) {
        ex = false;
      }
      ImGui::EndPopup();
    }
  }
}

void MainMenu::Show() {
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  // windowFlags |= ImGuiWindowFlags_NoBackground;
  windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  windowFlags |= ImGuiWindowFlags_NoResize;
  
  ImGui::SetNextWindowSize(ImVec2(Window::GetSize().x, Window::GetSize().y));
  ImGui::SetNextWindowPos(ImVec2(Window::GetPosition().x, Window::GetPosition().y));
  ImGui::Begin("Menu", nullptr, windowFlags);

  ImGui::SetCursorPosY(Window::GetSize().y / 2.0f - 100);

  switch (location) {
    case MenuLocation::Main:
      Main();
      break;
    case MenuLocation::Settings:
      Settings();
      break;
  }
  ImGui::End();
}