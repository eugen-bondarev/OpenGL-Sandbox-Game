#pragma once

#include "Asset.h"

class ImageAsset : public Asset {
public:
    ImageAsset(const Str& path);
    ImageAsset() = default;

    ~ImageAsset();

    virtual void Load(const Str& path) override;

    const unsigned char* GetData() const;
    Size GetSize() const;
    int GetChannels() const;

private:
    unsigned char* data;
    Size size;
    int amountOfChannels;
};