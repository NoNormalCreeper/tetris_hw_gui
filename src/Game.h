//
// Created by Rikka on 2025/5/16.
//

#ifndef GAME_H
#define GAME_H
#include <array>
#include <optional>
#include "Block.h"


class Game {
public:
    // const config
    static constexpr int game_height = 20;
    static constexpr int game_width = 10;
    static constexpr int death_height = 16;    // 堆叠到此高度时游戏结束

    // 游戏数据
    std::array<std::array<std::optional<char>, game_width>, game_height> game_board; // 游戏区域
    int score = 0; // 当前分数，负数代表游戏结束的最终得分
    Block& next_block;

    // 游戏当前操作状态
    Action current_action;
};



#endif //GAME_H
