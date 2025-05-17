//
// Created by Rikka on 2025/5/16.
//

#include "Block.h"

namespace k_Block {
    const auto I = Block('I', "cyan", {Pos(0, 0), Pos(1, 0), Pos(2, 0), Pos(3, 0)});

    const auto list = std::vector<Block>{I};
}


const Block& Block::getBlockByLabel(const char label) const {
    for (const auto &block : k_Block::list) {
        if (block.label == label) {
            return block;
        }
    }
    throw std::runtime_error("Block label '" + std::to_string(label) + "' not found");
}
