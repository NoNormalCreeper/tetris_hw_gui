//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"

Game::Game()
    : game_board(std::array<std::array<std::optional<char>, game_width>,
                            game_height>()), m_is_game_over(false),
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

bool Game::isValidAction(const Action& action) const {
    const auto block = action.block;
    const auto board_anchor = action.anchor;

    if (!block || block->occupied.empty()) {
        return false; // 无效方块指针
    }

    for (const auto& occupied_rel_pos : block->occupied) {
        // 绝对坐标 = 板上锚点 + (方块内部相对坐标 - 方块内部锚点)
        const Pos game_pos = board_anchor + (occupied_rel_pos - block->anchor);

        // 检查是否越左右边界
        if (game_pos.x < 0 || game_pos.x >= game_width) {
            return false;
        }
        // 检查是否越上下边界
        if (game_pos.y < 0 || game_pos.y >= game_height) {
            return false;
        }
        // 检查位置是否已被其他方块占据
        if (game_board.at(game_pos.x).at(game_pos.x).has_value()) {
            return false;
        }
    }
    return true;
}

bool Game::tryMoveLeft() {
    if (m_is_game_over || !current_action.block)
        return false;

    const Pos new_anchor = current_action.anchor + Pos(-1, 0);
    if (isValidAction({current_action.block, new_anchor})) {
        current_action.anchor = new_anchor;
        return true;
    }
    return false;
}

bool Game::tryMoveRight() {
    if (m_is_game_over || !current_action.block)
        return false;

    const Pos new_anchor = current_action.anchor + Pos(1, 0);
    if (isValidAction({current_action.block, new_anchor})) {
        current_action.anchor = new_anchor;
        return true;
    }
    return false;
}

bool Game::moveDown() {
    if (m_is_game_over || !current_action.block)
        return false;

    const Pos new_anchor = current_action.anchor + Pos(0, 1);
    if (isValidAction({current_action.block, new_anchor})) {
        current_action.anchor = new_anchor;
        return true;
    }
    return false;
}

bool Game::tryRotate() {
    if (m_is_game_over || !current_action.block)
        return false;

    Block rotated_block =
        current_action.block->rotate(); // 获取旋转后方块的副本

    if (isValidAction({&rotated_block, current_action.anchor})) {
        std::copy(current_action.block->occupied.begin(),
                  current_action.block->occupied.end(),
                  rotated_block.occupied.begin());
        return true;
    }
    return false; 
}
