#pragma once

#include "Asset.h"

#include "Util/Structures.h"

class ImageAsset : public Asset {
public:
    ImageAsset(const std::string& path);
    ImageAsset() = default;

    ~ImageAsset();

    virtual void Load(const std::string& path) override;

    const unsigned char* GetData() const;
    Size GetSize() const;
    int GetChannels() const;

private:
    unsigned char* data;
    Size size;
    int amountOfChannels;
};