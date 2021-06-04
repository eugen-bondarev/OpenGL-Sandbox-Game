#pragma once

class Asset {
public:
    virtual void Load(const Str& path) = 0;
};