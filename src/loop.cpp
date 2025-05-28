//
// Created by Rikka on 2025/5/15.
//

#include "mainwindow.h"

// 以下函数已经添加到了Game.h里：
// bool Game::isGameOver();
// bool Game::moveDown(); // 尝试将当前方块下落一格，落不动时返回false
// void Game::lockCurrentBlock(); // 将当前方块固定到棋盘上
// void Game::clearFullRows(); // 检查并消除满行，加分
// void Game::spawnNewBlock(); // 产生新的当前方块和下一个方块

void Ui::MainWindow::onTimeOut(Context& ctx)
{
    if (ctx.game.isGameOver()) {
        ctx.status = GAME_OVER;
        syncBoardAndActionToUi();
        return;
    }

    // 主循环：自动下落处理
    bool move_success = ctx.game.moveDown(); // moveDown() 成功返回true，否则为false

    if (!move_success) {
        // 下落不了，说明到底或被阻挡，需锁定方块并处理消行
        ctx.game.placeCurrentBlock();
        ctx.game.clearFullRows(); // 有消行要加分的加分
        setScoreWidgetNumber(abs(ctx.game.score)); // 更新分数显示

        // 生新块
        ctx.game.spawnNewBlock();
        setNextBlockWidget(ctx.game.next_block->color);
    }

    // 渲染同步刷新
    syncBoardAndActionToUi();
}
