//
// Created by Rikka on 2025/5/16.
//

#ifndef GAME_H
#define GAME_H
#include <array>


class Game {
public:
    static constexpr int game_height = 20;
    static constexpr int game_width = 10;
    static constexpr int death_height = 16;    // 堆叠到此高度时游戏结束
    std::array<std::array<char, game_width>, game_height> game_board; // 游戏区域

};



#endif //GAME_H
