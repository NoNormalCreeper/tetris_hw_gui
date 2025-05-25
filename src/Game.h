//
// Created by Rikka on 2025/5/16.
//

#ifndef GAME_H
#define GAME_H
#include "Block.h"
#include <array>
#include <optional>
#include <algorithm>
#include <vector>
#include <random> 

class Game {
public:
    // const config
    static constexpr int game_height = 20;
    static constexpr int game_width = 10;
    static constexpr int death_height = 16; // 堆叠到此高度时游戏结束

    // 游戏数据
    std::array<std::array<std::optional<char>, game_width>, game_height>
        game_board; // 游戏区域
    int score = 0;  // 当前分数，负数代表游戏结束的最终得分
    const Block* next_block;

    // 游戏当前操作状态
    Action current_action;

    Game(); // TODO

    // --- 需要新增的函数 --- 
    bool tryMoveLeft();      // 尝试左移当前方块
    bool tryMoveRight();     // 尝试右移当前方块
    bool tryRotate();        // 尝试旋转当前方块
    bool moveDown();         // 尝试将当前方块下落一格
    void lockCurrentBlock(); // 将当前方块固定到棋盘上
    void clearFullRows();    // 检查并消除满行，增加分数
    void spawnNewBlock();    // 生成新的当前方块和下一个方块
    bool isGameOver() const; // 检查游戏是否结束


private:
    const Action&
    setInitAction(const Block* current_block); // 根据当前方块获取初始的方块位置

    // 游戏结束标志
    bool m_is_game_over;  

    // --- 需要新增的辅助函数 ---
    const Block* getRandomBlock(); // 随机生成一个方块
    bool isValidAction(const Action& action) const; // 检查方块在指定位置是否有效
};


#endif //GAME_H
