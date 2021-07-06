#pragma once

#include "asset.h"

namespace mw {

class TextAsset : public Asset
{
public:
	TextAsset(const std::string &path);
	TextAsset() = default;

	virtual void Load(const std::string &path) override;

	std::string GetContent() const;

private:
	std::string content;
};

}