#pragma once

#include "asset.h"

namespace Werwel {

class TextAsset : public Asset
{
public:
	TextAsset(const std::string &path, const std::string& root = "");
	TextAsset() = default;

	virtual void Load(const std::string &path, const std::string& root = "") override;

	std::string GetContent() const;

private:
	std::string content;
};

}