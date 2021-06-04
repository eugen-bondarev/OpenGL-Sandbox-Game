#include "ImageAsset.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

ImageAsset::ImageAsset(const Str& path) {
	Load(path);
}

void ImageAsset::Load(const Str& path) {
	int width, height;
	data = stbi_load((Str(FORGIO_ROOT) + path).c_str(), &width, &height, &amountOfChannels, 0);
	size = { static_cast<float>(width), static_cast<float>(height) };
}

ImageAsset::~ImageAsset() {
	stbi_image_free(data);
}

const unsigned char* ImageAsset::GetData() const {
	return data;
}

Size ImageAsset::GetSize() const {
	return size;
}

int ImageAsset::GetChannels() const {
	return amountOfChannels;
}