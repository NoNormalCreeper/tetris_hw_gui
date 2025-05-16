//
// Created by Rikka on 2025/5/16.
//

#include "Block.h"

template<typename... Tuples>
Block::Block(const char label, QString color, Tuples... tuples)
    : label(label), color(std::move(color)), occupied{std::forward<Tuples>(tuples)...} {}
