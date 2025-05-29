// Created by Rikka on 2025/5/16.
//

#include "Game.h"

Game::Game()
    : game_board(std::array<std::array<std::optional<char>, game_width>,
                            game_height>()), m_is_game_over(false),next_block(nullptr),
    current_action(), falling_block_buf(k_Block::O)  // 这里随便初始化一个Block，不影响后续功能
    {
    // 初始化游戏区域
    for (auto& row : game_board) {
        row.fill(std::nullopt);
    }

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    //  防止 Block list 为空导致非法访问
    if (k_Block::list.empty()) {
        throw std::runtime_error("Block list is empty.");
    }

    next_block = getRandomBlock();
    // 防止 next_block 为 nullptr
    if (!next_block) {
        throw std::runtime_error("Get random block failed.");
    }
    spawnNewBlock();   // spawnNewBlock 会完成 falling_block_buf和current_action.block初始化

    // setInitAction(next_block); // 建议删除：这会把 current_action.block 指向 next_block（可能带来野指针）
    // ......
}

const Block* Game::getRandomBlock() {
    if (k_Block::list.empty()) {
        // throw std::runtime_error("Block list is empty."); // 放构造函数判定即可
        return nullptr; // 防止空表，返回 nullptr
    }

    int index = std::rand() % k_Block::list.size();
    return &k_Block::list[index];
}

// 建议不再需调用 setInitAction，后续用 buildInitAction 替代更安全。保留原函数实现，但不要再外部主动调。
// const Action& Game::setInitAction(const Block* current_block) {
//     this->current_action = Action{
//         current_block, Pos(game_width / 2, game_height - 3)
//     };
//     return this->current_action;
// }

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
        // 安全旋转，替换缓冲区实体为新朝向，并重新绑定指针
        falling_block_buf = rotated_block;
        current_action.block = &falling_block_buf;
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
    // 检查是否因放置方块而导致堆叠过高 -> 游戏结束

    for (int x = 0; x < game_width; ++x) {
        if (game_board[death_height -1][x].has_value()) {
            m_is_game_over = true;
            break;
        }
     }
}

void Game::clearFullRows() {
    int rows_cleared = 0;
    std::array<std::array<std::optional<char>, game_width>, game_height> new_board;
    int new_row = game_height - 1; // 从底部开始填充
    
    for (int y = game_height - 1; y >= 0; --y) {
        bool row_is_full = true;
        for (int x = 0; x < game_width; ++x) {
            if (!game_board[y][x].has_value()) {
                row_is_full = false;
                break;
            }
        }
        
        if (!row_is_full) {
            // 保留非满行
            new_board[new_row] = game_board[y];
            new_row--;
        } else {
            rows_cleared++;
        }
    }
    
    // 填充顶部空行
    for (int y = new_row; y >= 0; --y) {
        new_board[y].fill(std::nullopt);
    }
    
    // 更新游戏板
    game_board = new_board;

    // 更新分数
    if (rows_cleared > 0) {
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
    if (!next_block) {
        m_is_game_over = true;
        if (score >= 0) score = -score;
        return;
    }

    falling_block_buf = *next_block;
    current_action.block = &falling_block_buf;
    current_action.anchor = Pos(game_width / 2, game_height - 2); // 调整初始位置

    next_block = getRandomBlock();

    if (!isValidAction(current_action)) {
        m_is_game_over = true;
        if (score >= 0) score = -score;
    }
}
