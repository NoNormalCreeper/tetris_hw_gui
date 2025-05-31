#ifndef GAME_H
#define GAME_H
#include "Block.h"
#include <array>
#include <optional>
#include <algorithm>
#include <vector>
#include <ctime>
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

    // const Block* next_block = nullptr; // 确保有明确的初始值，防止悬空指针
    std::unique_ptr<Block> next_block; // 下一个方块，使用智能指针避免悬空指针

    // 游戏当前操作状态
    Action current_action;

    Game();

    bool tryMoveLeft();      // 尝试左移当前方块
    bool tryMoveRight();     // 尝试右移当前方块
    bool tryRotate();        // 尝试旋转当前方块
    bool moveDown();         // 尝试将当前方块下落一格
    void placeCurrentBlock();   // 将当前方块固定到棋盘上
    void clearFullRows();    // 检查并消除满行，增加分数
    void spawnNewBlock();    // 生成新的当前方块和下一个方块
    bool isGameOver() const; // 检查游戏是否结束

    // 新增：统一设置当前活动方块，避免悬空指针和临时变量指针问题
    void setCurrentBlock(std::unique_ptr<Block> block, const Pos& anchor);

private:
    //为了避免指针悬空或失效，不再使用setInitAction
    //current_action.block 需要指向正在活动的 falling_block_buf
    //而setInitAction会无条件让 current_action.block 指向传进来的 Block 指针
    //  const Action& setInitAction(const Block* current_block);   // 已注销

    // 游戏结束标志
    bool m_is_game_over = false;  // 添加初始化，防止未初始化即读取

    // --- 辅助函数 ---
    static std::unique_ptr<Block>getRandomBlock(); // 随机生成一个方块
    bool isValidAction(const Block& block, const Pos& anchor) const; // 检查方块在指定位置是否有效
};

#endif //GAME_H
