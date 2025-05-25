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
