#pragma once

#include "Util/Structures.h"
#include "BlockType.h"

struct BlockView {
    BlockType type { BlockType::Empty };

    bool lt { false }, mt { false }, rt { false },
         lm { false },               rm { false },
         lb { false }, mb { false }, rb { false }; // 8 byte

    BlockView() = default;
    BlockView(BlockType type) { 
        this->type = type; 
    }
};

struct Block : BlockView {
    Pos position;

    Block(BlockType type, Pos position) : BlockView(type) {
        this->position = position;
    }
};