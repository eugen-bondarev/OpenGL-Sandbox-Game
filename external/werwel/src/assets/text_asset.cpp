#include "assets/text_asset.h"

#include <fstream>

namespace ww {

TextAsset::TextAsset(const std::string &path, const std::string& root)
{
	Load(path, root);
}

void TextAsset::Load(const std::string &path, const std::string& root)
{
	std::ifstream file;
	std::string fullPath = root + path;

	file.open(fullPath);

	if (!file.is_open())
	{
		// Window::Minimize();
		// NF_ERROR_POPUP("Could not load a text asset: " + fullPath, "TextAsset::Load()");
		// NF_ERROR_EXIT();
	}

	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);
		content.append(line.empty() ? "\n" : line + '\n');
	}

	file.close();
}

std::string TextAsset::GetContent() const
{
	return content;
}

}