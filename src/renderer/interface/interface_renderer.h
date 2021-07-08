#pragma once

#include "game/subcomponents/inventory/inventory.h"

#include "mgcwel/mgcwel.h"

class InterfaceRenderer
{
public:
	InterfaceRenderer(Inventory &inventory);
	void Render();

private:
	// ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
	// windowFlags |= ImGuiWindowFlags_NoBackground;
	// windowFlags |= ImGuiWindowFlags_NoTitleBar;
	// windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	// windowFlags |= ImGuiWindowFlags_NoResize;
	// windowFlags |= ImGuiWindowFlags_NoScrollbar;
	// windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;
	void RenderSideMenu();
	void RenderBottomBar(
		const std::string &windowID, 
		bool* open,
		Vec2 position, 
		Vec2 amountOfButtons, 
		int startingIndex = 0,
		ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None
	);

	Inventory &inventory;

	bool inventoryOpen{false};

	InterfaceRenderer(const InterfaceRenderer &) = delete;
	InterfaceRenderer &operator=(const InterfaceRenderer &) = delete;
};