#pragma once

#include "asset.h"

namespace ww {

class ImageAsset : public Asset
{
public:
	ImageAsset(const std::string &path, const std::string& root = "");
	ImageAsset() = default;

	~ImageAsset();

	virtual void Load(const std::string &path, const std::string& root = "") override;

	const unsigned char *GetData() const;
	Vec2 GetSize() const;
	int GetChannels() const;

private:
	unsigned char *data{nullptr};
	Vec2 size;
	int amountOfChannels;
};

}