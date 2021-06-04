#include "TextAsset.h"

#include <fstream>

TextAsset::TextAsset(const Str& path) {
    Load(path);
}

void TextAsset::Load(const Str& path) {
    std::ifstream file;
#ifdef FORGIO_ROOT
    file.open(Str(FORGIO_ROOT) + path);
#else
    file.open(path);
#endif

    while (!file.eof()) {
        Str line;
        std::getline(file, line);
        content.append(line.empty() ? "\n" : line);
    }

    file.close();
}

Str TextAsset::GetContent() const {
    return content;
}
