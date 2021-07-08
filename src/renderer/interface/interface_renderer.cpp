#include "interface_renderer.h"

#include "renderer/world/map/map_renderer.h"

InterfaceRenderer::InterfaceRenderer(Inventory &inventory) : inventory{inventory}
{
}

void InterfaceRenderer::Render()
{
	RenderSideMenu();

	if (inventoryOpen)
	{
		RenderBottomBar("Inventory", &inventoryOpen, Vec2(0, 100), Vec2(10, 6), 10);
	}

	RenderBottomBar("BottomBar", nullptr, Vec2(0, 5), Vec2(10, 1), 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);
}

void InterfaceRenderer::RenderSideMenu()
{		
	TileMap* tm = TextureAtlas::Get<InterfaceIconTileMap>(TextureAtlasType::InterfaceIcons);
	Vec2 foo = TextureAtlas::Get<InterfaceIconTileMap>(TextureAtlasType::InterfaceIcons)->Get(InterfaceIconType::Bag);

	ImVec2 uv0 = ImVec2(Vec2(1.0f / tm->GetAmountOfTiles() * foo));
	ImVec2 uv1 = ImVec2(Vec2(1.0f / tm->GetAmountOfTiles() * (foo + 1.0f)));

	ImGui::SetNextWindowPos(ImVec2(20, mw::Window::GetSize().y - 50 - 20));
	ImGui::Begin("Side menu", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground);

	if (MW_KEY_PRESSED(MW_KEY_E))
	{
		inventoryOpen = !inventoryOpen;
	}
	
	if (ImGui::ImageButton((void *)(intptr_t)tm->GetHandle(), ImVec2(32, 32), uv0, uv1, -1))
	{
		inventoryOpen = !inventoryOpen;
	}
	ImGui::End();
}

void InterfaceRenderer::RenderBottomBar(const std::string &windowID, bool* open, Vec2 position, Vec2 amountOfButtons, int startingIndex, ImGuiWindowFlags windowFlags)
{	
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 2));

	windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	windowFlags |= ImGuiWindowFlags_NoCollapse;
	windowFlags |= ImGuiWindowFlags_NoResize;
	windowFlags |= ImGuiWindowFlags_NoScrollbar;
	windowFlags |= ImGuiWindowFlags_NoScrollWithMouse;

	static Vec2 buttonSize = Vec2(32.0f);
	static Vec2 padding = Vec2(4, 2);
	static Vec2 innerPadding = Vec2(4, 2);
	static Vec2 fullButtonSize = buttonSize + (innerPadding + padding) * 2.0f;
	Vec2 barSize = fullButtonSize * amountOfButtons + Vec2(0, windowFlags & ImGuiWindowFlags_NoTitleBar ? 0 : 20 /* title bar */) + (Vec2(ImGui::GetStyle().WindowPadding.x, ImGui::GetStyle().WindowPadding.y) - ImGui::GetStyle().WindowBorderSize) * 2.0f;
	Vec2 barPosition = Vec2((mw::Window::GetSize().x - barSize.x) / 2.0f, mw::Window::GetSize().y - barSize.y - position.y);

	ImGui::SetNextWindowPos(ImVec2(barPosition.x, barPosition.y));
	ImGui::SetNextWindowSize(ImVec2(barSize.x, barSize.y));

	ImGui::Begin(windowID.c_str(), open, windowFlags);

	int startPosX = ImGui::GetCursorPosX() + padding.x / 2.0f;
	int startPosY = ImGui::GetCursorPosY();

	int index = startingIndex;

	for (int i = 0; i < amountOfButtons.x; i++)
	{
		for (int j = 0; j < amountOfButtons.y; j++)
		{
			Vec2 pos = Vec2(startPosX + i * (buttonSize.x + (innerPadding.x + padding.x) * 2), startPosY + j * (buttonSize.y + (innerPadding.y + padding.y) * 2));

			ImGui::SetCursorPos(ImVec2(pos));

			if (!inventory.items[index]->IsEmpty() && inventory.items[index]->GetCurrentAmount())
			{
				Icon &icon = inventory.items[index]->GetIcon();
				TileMap *tileMap = icon.first;
				Vec2 tile = icon.second;

				ImVec2 uv0 = ImVec2(Vec2(1.0f / tileMap->GetAmountOfTiles() * tile));
				ImVec2 uv1 = ImVec2(Vec2(1.0f / tileMap->GetAmountOfTiles() * (tile + 1.0f)));

				ImGui::PushID(index);
				if (inventory.selectedItem == index)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
					ImGui::ImageButton((void *)(intptr_t)tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1, -1);
					ImGui::PopStyleColor();
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
					if (ImGui::ImageButton((void *)(intptr_t)tileMap->GetHandle(), ImVec2(buttonSize.x, buttonSize.y), uv0, uv1))
					{
						inventory.selectedItem = index;
					}

					ImGui::PopStyleColor();
				}
				ImGui::PopID();

				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
				{
					ImGui::SetDragDropPayload("DND_DEMO_CELL", &index, sizeof(int));
					ImGui::EndDragDropSource();
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
					{
						int ind = *static_cast<const int *>(payload->Data);
						inventory.Swap(ind, index);
					}
					ImGui::EndDragDropTarget();
				}

				if (inventory.items[index]->GetStack() != 1)
				{
					ImGui::SetCursorPos(ImVec2(pos));
					ImGui::Text(std::to_string(inventory.items[index]->GetCurrentAmount()).c_str());
				}
			}
			else
			{
				if (inventory.selectedItem == index)
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_Button]);
				}
				else
				{
					ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyle().Colors[ImGuiCol_ButtonActive]);
				}

				if (
					ImGui::ImageButton(
						(void *)(intptr_t)0,
						ImVec2(buttonSize.x, buttonSize.y),
						ImVec2(0, 0),
						ImVec2(1, 1),
						-1,
						ImVec4(0, 0, 0, 0),
						ImVec4(0, 0, 0, 0)))
				{
					inventory.selectedItem = index;
				}

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload *payload = ImGui::AcceptDragDropPayload("DND_DEMO_CELL"))
					{
						int ind = *static_cast<const int *>(payload->Data);
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

	ImGui::PopStyleVar();
}