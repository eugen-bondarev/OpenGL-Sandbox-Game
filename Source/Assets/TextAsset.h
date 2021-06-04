#pragma once

#include "Asset.h"

class TextAsset : public Asset {
public:
    TextAsset(const Str& path);
    TextAsset() = default;

    virtual void Load(const Str& path) override;

    Str GetContent() const;

private:
    Str content;
};