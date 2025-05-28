//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"

Game::Game()
    : game_board(std::array<std::array<std::optional<char>, game_width>,
                            game_height>()), m_is_game_over(false),next_block(nullptr),
      current_action() {
    // 初始化游戏区域
    for (auto& row : game_board) {
        row.fill(std::nullopt);
    }

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    next_block = getRandomBlock(); 
    spawnNewBlock();
    setInitAction(next_block);
    // ......
}

const Block* Game::getRandomBlock() {
    if (k_Block::list.empty()) {
        throw std::runtime_error("Block list is empty.");
    }
    
    int index = std::rand() % k_Block::list.size();
    return &k_Block::list[index];
}

const Action& Game::setInitAction(const Block* current_block) {
    this->current_action = Action{
        current_block, Pos(game_width / 2, game_height - 3) // TODO: 这里的 anchor 需要根据实际情况设置
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
        if (game_board.at(game_pos.y).at(game_pos.x).has_value()) {
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

    const Pos new_anchor = current_action.anchor - Pos(0, 1);
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

void Game::placeCurrentBlock() {
    if (!current_action.block) return; 

    for (const auto& cell_relative_pos : current_action.block->occupied) {
        Pos board_pos = current_action.anchor + (cell_relative_pos - current_action.block->anchor);
        if (board_pos.y >= 0 && board_pos.y < game_height &&
            board_pos.x >= 0 && board_pos.x < game_width) {
            game_board[board_pos.y][board_pos.x] = current_action.block->label;
        }
    }
    // 在此检查是否因放置方块而导致堆叠过高 -> 游戏结束
   
    for (int x = 0; x < game_width; ++x) {
        if (game_board[death_height -1][x].has_value()) { 
            m_is_game_over = true;
            break;
        }
     }
}

void Game::clearFullRows() {
    int rows_cleared = 0;
    for (int y = game_height - 1; y >= 0; --y) {
        bool row_is_full = true;
        for (int x = 0; x < game_width; ++x) {
            if (!game_board[y][x].has_value()) {
                row_is_full = false;
                break;
            }
        }

        if (row_is_full) {
            rows_cleared++;
            for (int r = y; r > 0; --r) {
                game_board[r] = game_board[r - 1];
            }
            
            game_board[0].fill(std::nullopt); 
            y++; 
        }
    }

    if (rows_cleared > 0) {
        // 根据消除的行数加分
        // 1行: 100, 2行: 300, 3行: 500, 4行: 800 (Tetris)
        if (rows_cleared == 1) score += 100;
        else if (rows_cleared == 2) score += 300;
        else if (rows_cleared == 3) score += 500;
        else if (rows_cleared >= 4) score += 800; 
    }
}

bool Game::isGameOver() const {
     
    
    return m_is_game_over;
}

void Game::spawnNewBlock() {
    current_action.block = next_block; // 当前方块为之前的下一个方块
    setInitAction(current_action.block); // 设置初始位置 

    next_block = getRandomBlock(); 

    // 检查新生成的方块是否有效，无效则游戏结束 
    if (!isValidAction(current_action)) {
        m_is_game_over = true;
        // 将 score 设为负数表示游戏结束时的最终得分
        if (score >= 0) score = -score;
    }
}

