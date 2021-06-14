#include "TextAsset.h"

#include <fstream>

TextAsset::TextAsset(const std::string& path) {
	Load(path);
}

void TextAsset::Load(const std::string& path) {
	std::ifstream file;
	file.open(FORGIO_ROOT + path);

	while (!file.eof()) {
		std::string line;
		std::getline(file, line);
		content.append(line.empty() ? "\n" : line);
	}

	file.close();
}

std::string TextAsset::GetContent() const {
	return content;
}
