#pragma once

#include "imgui/imgui.h"

#include "Werwel/Texture.h"

namespace ImGui {

static void MyImage(const std::string& name, Ref<Werwel::Texture>& texture, ImVec2 size = ImVec2(0, 0)) {
	ImGui::Begin(name.c_str());
    if (size.x == 0 || size.y == 0) {
      size = ImVec2(texture->GetSize().x, texture->GetSize().y);
    }
    ImGui::Image((void*)(intptr_t)texture->GetHandle(), size, ImVec2(0, 0), ImVec2(1, -1));
	ImGui::End();
}

}