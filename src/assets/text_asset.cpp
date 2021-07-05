#include "text_asset.h"

#include <fstream>

#include "core/window.h"

TextAsset::TextAsset(const std::string& path) {
	Load(path);
}

void TextAsset::Load(const std::string& path) {
	std::ifstream file;
	std::string fullPath = NF_ROOT + path;

	file.open(fullPath);

	if (!file.is_open()) {
		Window::Minimize();
		NF_ERROR_POPUP("Could not load a text asset: " + fullPath, "TextAsset::Load()");
		NF_ERROR_EXIT();
	}

	while (!file.eof()) {
		std::string line;
		std::getline(file, line);
		content.append(line.empty() ? "\n" : line + '\n');
	}

	file.close();
}

std::string TextAsset::GetContent() const {
	return content;
}
