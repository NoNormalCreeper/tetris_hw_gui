#include "Game.h"

Game::Game()
    : game_board(std::array<std::array<std::optional<char>, game_width>,
                            game_height>()),
      m_is_game_over(false), next_block(nullptr),
      current_action() // 这里随便初始化一个Block，不影响后续功能
{
    // 初始化游戏区域
    for (auto& row : game_board) {
        row.fill(std::nullopt);
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
    spawnNewBlock(); // spawnNewBlock 会完成
                     // falling_block_buf和current_action.block初始化

    // setInitAction(next_block); // 旧代码注释掉，不再使用，防止悬空指针
    // 初始化随机数生成器
    m_gen = std::mt19937(m_rd());
    m_dist = std::uniform_int_distribution<size_t>(0, k_Block::list.size() - 1);
}

// 新增统一接口管理 current_action.block
void Game::setCurrentBlock(std::unique_ptr<Block> block, const Pos& anchor) {
    // 使用 std::move 确保 block 的所有权转移
    current_action.block = std::move(block);
    current_action.anchor = anchor;
}

std::unique_ptr<Block> Game::getRandomBlock() {
    if (k_Block::list.empty()) {
        return nullptr; // 防止空表，返回 nullptr
    }

    return std::make_unique<Block>(k_Block::list.at(m_dist(m_gen)));
}

// const Action& Game::setInitAction(const Block* current_block) {
//     this->current_action = Action{
//         current_block, Pos(game_width / 2, game_height - 3)
//     };
//     return this->current_action;
// }

bool Game::isValidAction(const std::unique_ptr<Block>& block,
                         const Pos& anchor) const {
    if (block->occupied.empty()) {
        return false; // 无效方块指针
    }

    return std::all_of(
        block->occupied.begin(), block->occupied.end(),
        [&](const Pos& rel_pos) {
            const Pos game_pos = anchor + (rel_pos - block->anchor);
            return game_pos.x >= 0 && game_pos.x < game_width &&
                   game_pos.y >= 0 && game_pos.y < game_height &&
                   !game_board.at(game_pos.y).at(game_pos.x).has_value();
        });
}

bool Game::tryMoveLeft() {
    if (m_is_game_over || !current_action.block)
        return false;

    const Pos new_anchor = current_action.anchor + Pos(-1, 0);
    if (isValidAction(current_action.block, new_anchor)) {
        current_action.anchor = new_anchor;
        return true;
    }
    return false;
}

bool Game::tryMoveRight() {
    if (m_is_game_over || !current_action.block)
        return false;

    const Pos new_anchor = current_action.anchor + Pos(1, 0);
    if (isValidAction(current_action.block, new_anchor)) {
        current_action.anchor = new_anchor;
        return true;
    }
    return false;
}

bool Game::moveDown() {
    if (m_is_game_over || !current_action.block)
        return false;

    const Pos new_anchor = current_action.anchor - Pos(0, 1);
    if (isValidAction(current_action.block, new_anchor)) {
        current_action.anchor = new_anchor;
        return true;
    }
    return false;
}

bool Game::tryRotate() {
    if (m_is_game_over || !current_action.block)
        return false;

    auto rotated_block = current_action.block->rotate(); // 获取旋转后方块的副本

    if (isValidAction(rotated_block, current_action.anchor)) {
        current_action.block = std::move(rotated_block);
        current_action.anchor = current_action.anchor; // 保持锚点不变
        return true;
    }
    return false;
}

void Game::placeCurrentBlock() {
    if (!current_action.block)
        return;

    for (const auto& cell_relative_pos : current_action.block->occupied) {
        Pos board_pos = current_action.anchor +
                        (cell_relative_pos - current_action.block->anchor);
        if (board_pos.y >= 0 && board_pos.y < game_height && board_pos.x >= 0 &&
            board_pos.x < game_width) {
            game_board[board_pos.y][board_pos.x] = current_action.block->label;
        }
    }
    // 检查是否因放置方块而导致堆叠过高 -> 游戏结束

    /*for (int x = 0; x < game_width; ++x) {
        if (game_board[death_height -1][x].has_value()) {
            m_is_game_over = true;
            break;
        }
     }*/
    // 简单粗暴：
    // 在方块放置后，检查第16行是否有方块（y坐标=16）
    int critical_row = 16;

    if (critical_row >= 0 && critical_row < game_height) {
        m_is_game_over = std::any_of(
            game_board.at(critical_row).begin(),
            game_board.at(critical_row).end(),
            [](const std::optional<char>& cell) { return cell.has_value(); });
    }
}

void Game::clearFullRows() {
    std::vector<int> full_lines;
    full_lines.reserve(game_height);

    // find full lines
    for (int y = 0; y < game_height; ++y) {
        bool is_line_full = true;
        for (int x = 0; x < game_width; ++x) {
            if (!game_board.at(y).at(x).has_value()) {
                is_line_full = false;
                break;
            }
        }
        if (is_line_full) {
            full_lines.push_back(y);
        }
    }

    const auto rows_cleared = full_lines.size();
    if (rows_cleared == 0)
        return;

    // two pointers
    int write_row = 0;
    for (int read_row = 0; read_row < game_height; ++read_row) {
        bool is_full = false;
        for (const int full_y : full_lines) {
            if (read_row == full_y) {
                is_full = true;
                break;
            }
        }

        if (!is_full) {
            // 不是满行，则复制到写入位置
            if (write_row != read_row) {
                game_board.at(write_row) = game_board.at(read_row);
            }
            write_row++;
        }
    }

    // clear lines at top
    for (int y = write_row; y < game_height; ++y) {
        game_board.at(y).fill(std::nullopt); // 清空剩余行
    }

    // 更新分数
    if (rows_cleared == 1)
        score += 100;
    else if (rows_cleared == 2)
        score += 300;
    else if (rows_cleared == 3)
        score += 500;
    else
        score += 800;
}

bool Game::isGameOver() const { return m_is_game_over; }

void Game::spawnNewBlock() {
    if (!next_block) {
        m_is_game_over = true;
        if (score >= 0)
            score = -score;
        return;
    }

    // 使用新接口，统一赋值，避免指向临时变量或非法指针
    // setCurrentBlock(next_block, Pos(game_width / 2, game_height - 2));
    const auto block_height =
        std::max_element(next_block->occupied.begin(),
                         next_block->occupied.end(),
                         [](const Pos& a, const Pos& b) { return a.y < b.y; })
            ->y;
    current_action.anchor = Pos(game_width / 2 - next_block->anchor.x - 1,
                                game_height - block_height +
                                    next_block->anchor.y - 1); // 调整初始位置
    current_action.block = std::move(next_block);

    next_block = getRandomBlock();

    if (!isValidAction(current_action.block, current_action.anchor)) {
        m_is_game_over = true;
        if (score >= 0)
            score = -score;
    }

    /*
    // 旧代码
    falling_block_buf = *next_block;
    current_action.block = &falling_block_buf;
    current_action.anchor = Pos(game_width / 2, game_height - 2); // 调整初始位置
    next_block = getRandomBlock();
    if (!isValidAction(current_action)) {
        m_is_game_over = true;
        if (score >= 0) score = -score;
    }
    */
}
void Game::reset() {
    // 手动清空所有成员变量

    // 清空游戏棋盘
    for (auto& row : game_board)
        row.fill(std::nullopt);

    // 分数归零
    score = 0;

    // 游戏结束状态 false
    m_is_game_over = false;

    // current_action 重置
    current_action.reset();

    // 随机数引擎重新播种（重置m_gen等）
    m_gen.seed(m_rd()); // 正确做法
    m_dist = std::uniform_int_distribution<size_t>(0, k_Block::list.size() - 1);

    // next_block 设置为新的Block
    if (k_Block::list.empty()) {
        throw std::runtime_error("Block list is empty.");
    }
    next_block = getRandomBlock();

    if (!next_block) {
        throw std::runtime_error("Get random block failed.");
    }

    spawnNewBlock(); // 自动完成current_action.block初始化
}