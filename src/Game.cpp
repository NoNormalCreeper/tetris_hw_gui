//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"

Game::Game()
    : game_board(std::array<std::array<std::optional<char>, game_width>, game_height>()),
      next_block(),
      current_action() {
    // 初始化游戏区域
    for (auto & row : game_board) {
        row.fill(std::nullopt);
    }

    const auto first_block = k_Block::I;  // TODO: 改为正确的随机生成
    setInitAction(&first_block);
    // ......
}

const Action& Game::setInitAction(const Block* current_block) {
    this->current_action = Action{
        current_block,
        Pos(0, 0)   // TODO: 这里的 anchor 需要根据实际情况设置
    };
    return this->current_action;
}
