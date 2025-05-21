//
// Created by Rikka on 2025/5/16.
//

#include "Block.h"


const Block& Block::getBlockByLabel(const char label) {
    for (const auto &block : k_Block::list) {
        if (block.label == label) {
            return block;
        }
    }
    throw std::runtime_error("Block label '" + std::to_string(label) + "' not found");
}

Block Block::rotate() const {
    std::vector<Pos> new_occupied;
    new_occupied.reserve(this->occupied.size());

    for (const auto &pos: this->occupied) {
        new_occupied.emplace_back(
            this->anchor.x + (pos.y - this->anchor.y),
            this->anchor.y + (this->anchor.x - pos.x)
        );
    }

    return Block(this->label, this->color, new_occupied, this->anchor);
}
