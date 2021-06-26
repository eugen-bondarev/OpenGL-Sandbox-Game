#include "MainMenu.h"

#include "Core/Window.h"

#include "imgui/imgui.h"

MainMenu::MainMenu(Ref<Game>& game) : game { game } {

}

void MainMenu::Settings() {
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Back")) {
      location = MenuLocation::Main;
    }
}

void MainMenu::Main() {
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Start game")) {
      game = CreatePtr<Game>();
    }
  
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Settings")) {
      location = MenuLocation::Settings;
    }
  
  ImGui::SetCursorPosX(Window::GetSize().x / 2.0f);
    if (ImGui::Button("Exit")) {
      Window::Close();
    }
}

void MainMenu::Show() {
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  windowFlags |= ImGuiWindowFlags_NoBackground;
  windowFlags |= ImGuiWindowFlags_NoTitleBar;
  windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
  
  ImGui::SetNextWindowSize(ImVec2(Window::GetSize().x, Window::GetSize().y));
  ImGui::SetNextWindowPos(ImVec2(0, 0));
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