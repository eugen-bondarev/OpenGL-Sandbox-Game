#include "console.h"

#include "imgui/imgui.h"

namespace Console
{
	std::vector<Message> Messages;

	Message::Message(MessageType type, const std::string& content) : Type{type}, Content{content}
	{

	}

	void PushMessage(MessageType type, const std::string& message)
	{
		Messages.emplace_back(type, message);

		if (Messages.size() > MAX_SIZE)
		{
			Messages.erase(std::begin(Messages));
		}
	}

	void PushInfo(const std::string& message)
	{
		PushMessage(MessageType::Info, message);
	}

	void PushWarning(const std::string& message)
	{
		PushMessage(MessageType::Warning, message);
	}

	void PushError(const std::string& message)
	{
		PushMessage(MessageType::Error, message);
	}	

	void Render()
	{
		static ImVec2 window_size = ImVec2(500, 200);
		static ImVec2 padding = ImVec2(20, 20);
		static ImVec2 window_pos = ImVec2(MW_WINDOW_WIDTH() - padding.x - window_size.x, MW_WINDOW_HEIGHT() - padding.y - window_size.y);

		ImGui::SetNextWindowSize(window_size);
		ImGui::SetNextWindowPos(window_pos);
		ImGui::Begin("Console");
			for (int i = 0; i < Messages.size(); i++)
			{
				ImGui::PushID(i);
					ImGui::Text(Messages[i].Content.c_str());
				ImGui::PopID();
			}
		ImGui::End();
	}
}