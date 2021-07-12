#pragma once

#include <string>
#include <vector>

namespace Console
{
	static constexpr unsigned int MAX_SIZE = 100;

	enum class MessageType
	{
		Info = 0,
		Warning,
		Error
	};

	struct Message
	{
		MessageType Type;
		std::string Content;

		Message(MessageType type, const std::string& content);
	};

	extern std::vector<Message> Messages;

	void PushMessage(MessageType type, const std::string& message);
	void PushInfo(const std::string& message);
	void PushWarning(const std::string& message);
	void PushError(const std::string& message);

	void Render();
}