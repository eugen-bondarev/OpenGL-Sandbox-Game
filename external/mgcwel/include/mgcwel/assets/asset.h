#pragma once

namespace mw {

class Asset
{
public:
	virtual void Load(const std::string &path) = 0;
};

}