#include "main_menu.h"

MainMenu::MainMenu(Ref<Game> &game) : game{game}
{
}

void MainMenu::Settings()
{
	float line = mw::Window::GetSize().x / 2.0f - 110.0f;

	ImGui::SetCursorPosX(line);
	ImGui::PushFont(mw::Gui::titleFont);
	ImGui::Text("Settings");
	ImGui::PopFont();

	static Vec2i s = {mw::Window::GetSize().x, mw::Window::GetSize().y};

	const char *resolutions[] =
		{
			"1920 x 1080",
			"1366 x 768",
			"1024 x 768",
			"800 x 600",
			"640 x 480",
		};

	std::array<Vec2, 5> sizes = {
		Vec2(1920, 1080),
		Vec2(1366, 768),
		Vec2(1024, 768),
		Vec2(800, 600),
		Vec2(640, 480),
	};

	static int currentResolution = 0;

	ImGui::PushItemWidth(150);
	ImGui::SetCursorPosX(line);
	if (ImGui::Combo("Resolution", &currentResolution, resolutions, 5))
	{
		mw::Window::GetCurrentWindowSettings().size = sizes[currentResolution];
	}

	const char *items[] = {"Fullscreen", "Borderless", "Windowed"};
	static int item_current = static_cast<int>(mw::Window::GetCurrentWindowSettings().mode);

	ImGui::SetCursorPosX(line);
	if (ImGui::Combo("Window mode", &item_current, items, 3))
	{
		mw::Window::GetCurrentWindowSettings().mode = static_cast<mw::Window::Mode>(item_current);
	}

	ImGui::SetCursorPosX(line);
	ImGui::Checkbox("Vertical synchronization", &mw::Window::GetCurrentWindowSettings().vSync);
	ImGui::PopItemWidth();

	ImGui::SetCursorPosX(line);
	if (ImGui::Button("Back"))
	{
		location = MenuLocation::Main;
	}
	ImGui::SameLine();
	if (ImGui::Button("Apply"))
	{
		mw::Window::Recreate(mw::Window::GetCurrentWindowSettings());
	}
}

void MainMenu::Main()
{
	float line = mw::Window::GetSize().x / 2.0f - 110.0f;

	ImGui::SetCursorPosX(line);
	ImGui::PushFont(mw::Gui::titleFont);

	ImGui::Text("NaturaForge");

	ImGui::PopFont();

	static int seed = 27655;

	ImGui::SetNextItemWidth(100);
	ImGui::SetCursorPosX(line);

	ImGui::InputScalar("Seed", ImGuiDataType_U32, &seed, nullptr, nullptr);

	ImGui::SetCursorPosX(line);
	if (ImGui::Button("Start game"))
	{
		game = CreatePtr<Game>(seed);
	}

	ImGui::SetCursorPosX(line);
	if (ImGui::Button("Settings"))
	{
		location = MenuLocation::Settings;
	}

	static bool exitPopup = false;

	ImGui::SetCursorPosX(line);
	if (ImGui::Button("Exit"))
	{
		exitPopup = true;
	}

	if (exitPopup)
	{
		static const ImVec2 windowSize = ImVec2(200, 80);
		static const ImVec2 buttonSize = ImVec2(50, 25);
		static const int sidePadding = 25;

		ImGui::SetNextWindowSize(windowSize);
		ImGui::OpenPopup("Are you sure?");
		if (ImGui::BeginPopupModal("Are you sure?", NULL, ImGuiWindowFlags_NoResize))
		{
			ImGui::SetCursorPosX(sidePadding);
			if (ImGui::Button("Yes", buttonSize))
			{
				mw::Window::Close();
			}

			ImGui::SameLine();

			ImGui::SetCursorPosX(windowSize.x - sidePadding - buttonSize.x);
			if (ImGui::Button("No", buttonSize))
			{
				exitPopup = false;
			}
			ImGui::EndPopup();
		}
	}
}

void MainMenu::Show()
{
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 15));

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

	ImGui::PopStyleVar();
}