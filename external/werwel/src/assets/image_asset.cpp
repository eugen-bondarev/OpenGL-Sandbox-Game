#include "assets/image_asset.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

namespace ww {

ImageAsset::ImageAsset(const std::string &path, const std::string& root)
{
	Load(path, root);
}

void ImageAsset::Load(const std::string &path, const std::string& root)
{
	int width, height;
	std::string fullPath = root + path;
	data = stbi_load(fullPath.c_str(), &width, &height, &amountOfChannels, 0);
	if (!data)
	{
		// NF_ERROR_POPUP("Could not load an image asset: " + fullPath, "ImageAsset::Load()");
		// NF_ERROR_EXIT();
	}
	size = {static_cast<float>(width), static_cast<float>(height)};
}

ImageAsset::~ImageAsset()
{
	stbi_image_free(data);
}

const unsigned char *ImageAsset::GetData() const
{
	return data;
}

Vec2 ImageAsset::GetSize() const
{
	return size;
}

int ImageAsset::GetChannels() const
{
	return amountOfChannels;
}

}