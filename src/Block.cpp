//
// Created by Rikka on 2025/5/16.
//

#include "Block.h"

const Block& Block::getBlockByLabel(const char label) {
    for (const auto& block : k_Block::list) {
        if (block.label == label) {
            return block;
        }
    }
    throw std::runtime_error("Block label '" + std::to_string(label) +
                             "' not found");
}

// Returns a COPY of the block rotated 90 degrees
std::unique_ptr<Block> Block::rotate() const {
    std::vector<Pos> new_occupied;
    new_occupied.reserve(this->occupied.size());

    for (const auto& pos : this->occupied) {
        // 修正旋转公式：顺时针旋转90度 (x,y) -> (y, -x)
        int relative_x = pos.x - anchor.x;
        int relative_y = pos.y - anchor.y;
        int new_x = anchor.x - relative_y;
        int new_y = anchor.y + relative_x;
        new_occupied.emplace_back(new_x, new_y);
    }

    return std::make_unique<Block>(this->label, this->color,
                                   std::move(new_occupied), this->anchor);
}
