//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"

Game::Game()
    : game_board(std::array<std::array<std::optional<char>, game_width>,
                            game_height>()),
      next_block(), current_action() {
    // 初始化游戏区域
    for (auto& row : game_board) {
        row.fill(std::nullopt);
    }

    const auto first_block = k_Block::I; // TODO: 改为正确的随机生成
    setInitAction(&first_block);
    // ......
}

const Action& Game::setInitAction(const Block* current_block) {
    this->current_action = Action{
        current_block, Pos(0, 0) // TODO: 这里的 anchor 需要根据实际情况设置
    };
    return this->current_action;
}


bool Game::isValidPosition(const Block* block_to_check, const Pos& board_anchor) const {
    if (!block_to_check || block_to_check->occupied.empty()) { // 增加对空方块或无占用单元格方块的检查
         return false; // 无效方块指针或方块无实体
    }

    for (const auto& occupied_rel_pos : block_to_check->occupied) {
        // 计算方块中每个被占据单元格在游戏板上的绝对坐标：
        // 绝对坐标 = 板上锚点 + (方块内部相对坐标 - 方块内部锚点)
        Pos game_pos = board_anchor + (occupied_rel_pos - block_to_check->anchor);

        // 检查是否越出游戏板左右边界
        if (game_pos.x < 0 || game_pos.x >= game_width) {
            return false; // X 坐标越界
        }
        // 检查是否越出游戏板上下边界
        // 注意: Y 坐标可以等于 game_height 吗？通常不行，最大是 game_height - 1
        if (game_pos.y < 0 || game_pos.y >= game_height) {
            return false; // Y 坐标越界
        }
        // 检查游戏板上对应位置是否已被其他固定方块占据
        if (game_board[game_pos.y][game_pos.x].has_value()) {
            return false; // 与固定方块重叠
        }
    }
    return true; // 所有单元格位置均有效
}

bool Game::isValidPosition(const Block* block_to_check, const Pos& board_anchor) const {
    if (!block_to_check || block_to_check->occupied.empty()) { 
         return false; // 无效方块指针
    }

    for (const auto& occupied_rel_pos : block_to_check->occupied) {
        
        // 绝对坐标 = 板上锚点 + (方块内部相对坐标 - 方块内部锚点)
        Pos game_pos = board_anchor + (occupied_rel_pos - block_to_check->anchor);

        // 检查是否越左右边界
        if (game_pos.x < 0 || game_pos.x >= game_width) {
            return false; 
        }
        // 检查是否越上下边界
        if (game_pos.y < 0 || game_pos.y >= game_height) {
            return false; 
        }
        // 检查位置是否已被其他方块占据
        if (game_board[game_pos.y][game_pos.x].has_value()) {
            return false; 
        }
    }
    return true; 
}

bool Game::tryMoveLeft() {
    if (m_is_game_over || !current_action.block) return false; 

    Pos new_anchor = current_action.anchor + Pos(-1, 0); 
    if (isValidPosition(&m_current_block_instance, new_anchor)) {
        current_action.anchor = new_anchor; 
        return true; 
    }
    return false; 
}

bool Game::tryMoveRight() {
    if (m_is_game_over || !current_action.block) return false;

    Pos new_anchor = current_action.anchor + Pos(1, 0); 
    if (isValidPosition(&m_current_block_instance, new_anchor)) {
        current_action.anchor = new_anchor; 
        return true; 
    }
    return false; 
}

bool Game::moveDown() {
    if (m_is_game_over || !current_action.block) return false;

    Pos new_anchor = current_action.anchor + Pos(0, 1); 
    if (isValidPosition(&m_current_block_instance, new_anchor)) {
        current_action.anchor = new_anchor; 
        return true; 
    }
    return false; 
}

bool Game::tryRotate() {
    if (m_is_game_over || !current_action.block) return false;

    Block rotated_block_candidate = m_current_block_instance.rotate(); // 获取旋转后方块的副本
    
    if (isValidPosition(&rotated_block_candidate, current_action.anchor)) {
        m_current_block_instance = rotated_block_candidate; 
        return true;
    }
    return false; 
}
