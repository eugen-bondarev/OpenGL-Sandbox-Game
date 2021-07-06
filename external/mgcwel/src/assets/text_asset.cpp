#include "assets/text_asset.h"

#include <fstream>

namespace mw {

TextAsset::TextAsset(const std::string &path)
{
	Load(path);
}

void TextAsset::Load(const std::string &path)
{
	std::ifstream file;
	std::string fullPath = ROOT_DIR + path;

	file.open(fullPath);

	if (!file.is_open())
	{
		// Window::Minimize();
		// MW_ERROR_POPUP("Could not load a text asset: " + fullPath, "TextAsset::Load()");
		// MW_ERROR_EXIT();
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