#pragma once

namespace ww {

class Asset
{
public:
	virtual void Load(const std::string &path, const std::string& root = "") = 0;
};

}