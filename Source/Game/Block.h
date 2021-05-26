#pragma once

#include "Util/Structures.h"
#include "BlockType.h"

struct BlockView {
    BlockType type { BlockType::Empty };

    bool lt { false }, mt { false }, rt { false },
         lm { false },               rm { false },
         lb { false }, mb { false }, rb { false }; // 8 byte

    inline BlockView(BlockType type = BlockType::Empty) { 
        this->type = type; 
    }
};

struct Block : BlockView {
    Pos position;

    inline Block(BlockType type = BlockType::Empty, Pos position = Pos()) : BlockView(type) {
        this->position = position;
    }
};