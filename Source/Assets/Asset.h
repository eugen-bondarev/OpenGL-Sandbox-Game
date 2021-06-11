#pragma once

class Asset {
public:
	virtual void Load(const std::string& path) = 0;
};