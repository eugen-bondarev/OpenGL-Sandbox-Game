#pragma once

#include "asset.h"

#include "pch.h"

namespace mw {

class ImageAsset : public Asset
{
public:
	ImageAsset(const std::string &path);
	ImageAsset() = default;

	~ImageAsset();

	virtual void Load(const std::string &path) override;

	const unsigned char *GetData() const;
	Vec2 GetSize() const;
	int GetChannels() const;

private:
	unsigned char *data{nullptr};
	Vec2 size;
	int amountOfChannels;
};

}