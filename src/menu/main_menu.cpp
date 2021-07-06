#include "main_menu.h"

MainMenu::MainMenu(Ref<Game> &game) : game{game}
{
}

void MainMenu::Settings()
{
	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	ImGui::PushFont(mw::Gui::titleFont);
	ImGui::Text("Settings");
	ImGui::PopFont();

	static mw::Window::Settings windowSettings = {};

	static Vec2i s = {mw::Window::GetSize().x, mw::Window::GetSize().y};

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::Button("Recreate the window"))
	{
		mw::Window::Recreate(windowSettings);
	}

	ImGui::PushItemWidth(150);
	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::InputInt("Width", &s.x))
	{
		windowSettings.size = s;
	}
	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::InputInt("Height", &s.y))
	{
		windowSettings.size = s;
	}

	const char *items[] = {"Fullscreen", "Borderless", "Windowed"};
	static int item_current = static_cast<int>(windowSettings.mode);

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::Combo("Window mode", &item_current, items, 3))
	{
		windowSettings.mode = static_cast<mw::Window::Mode>(item_current);
	}

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	ImGui::Checkbox("Maximize window", &windowSettings.maximize);

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	ImGui::Checkbox("vSync", &windowSettings.vSync);
	ImGui::PopItemWidth();

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::Button("Back"))
	{
		location = MenuLocation::Main;
	}
}

void MainMenu::Main()
{
	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	ImGui::PushFont(mw::Gui::titleFont);
	ImGui::Text("NaturaForge");
	ImGui::PopFont();

	static int seed = 27655;

	ImGui::SetNextItemWidth(100);
	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	ImGui::InputInt("Seed", &seed);

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::Button("Start game"))
	{
		game = CreatePtr<Game>(seed);
	}

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::Button("Settings"))
	{
		location = MenuLocation::Settings;
	}

	static bool exitPopup = false;

	ImGui::SetCursorPosX(mw::Window::GetSize().x / 2.0f);
	if (ImGui::Button("Exit"))
	{
		exitPopup = true;
	}

	if (exitPopup)
	{
		ImGui::SetNextWindowSize(ImVec2(300, 100));
		ImGui::OpenPopup("Are you sure?");
		if (ImGui::BeginPopupModal("Are you sure?", NULL))
		{
			if (ImGui::Button("Yes"))
			{
				mw::Window::Close();
			}

			ImGui::SameLine();
			if (ImGui::Button("No"))
			{
				exitPopup = false;
			}
			ImGui::EndPopup();
		}
	}
}

void MainMenu::Show()
{
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
	windowFlags |= ImGuiWindowFlags_NoBackground;
	windowFlags |= ImGuiWindowFlags_NoTitleBar;
	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlags |= ImGuiWindowFlags_NoResize;

	ImGui::SetNextWindowSize(ImVec2(mw::Window::GetSize().x, mw::Window::GetSize().y));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Menu", nullptr, windowFlags);

	ImGui::SetCursorPosY(mw::Window::GetSize().y / 2.0f - 100);

	switch (location)
	{
	case MenuLocation::Main:
		Main();
		break;
	case MenuLocation::Settings:
		Settings();
		break;
	}
	ImGui::End();
}