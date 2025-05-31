//
// Created by Rikka on 2025/5/22.
//

#include "mainwindow.h" // 包含了 Context.h, Game.h (通过头文件链式依赖)
#include <QKeyEvent>

// 假设 Game 类中存在的
// - bool moveDown(): 方块下落一格，若无法下落则返回 false
// - void lockCurrentBlock(): 将当前方块固定到棋盘上
// - void clearFullRows(): 检查并消除满行，更新分数
// - void spawnNewBlock(): 生成新的当前方块和下一个方块
// - bool isGameOver(): 检查游戏是否结束
// - bool tryMoveLeft();
// - bool tryMoveRight();
// - bool tryRotate();

// 定义游戏速度，后续可以调整或设为动态变化 (例如，与等级挂钩)
constexpr int GAME_TIMER_INTERVAL_MS = 500; // 游戏计时器间隔，单位：毫秒

void Ui::MainWindow::keyPressEvent(QKeyEvent* event) {
    if (!event) {
        return;
    }

    switch (context.status) {
    case MAIN_MENU: // 主菜单状态
        if (event->key() == Qt::Key_Return ||
            event->key() == Qt::Key_Enter) // 回车键开始游戏
        {
            context.status = PLAYING;
            syncMenuStatusToUi();
            syncBoardAndActionToUi();
            timer.start(GAME_TIMER_INTERVAL_MS);
        }
        break;

    case PLAYING: // 游戏进行中状态
        // 新增暂停状态
        if (event->key() == Qt::Key_P) {
            context.status = PAUSE;
            timer.stop();
            syncBoardAndActionToUi(); // 可选: 更新UI以显示“已暂停”信息或遮罩
            return;
        }

        if (!context.game.current_action.block) {
            // 理论上不应出现

            return;
        }

        switch (event->key()) {
        case Qt::Key_Left: // 左移
        case Qt::Key_A:
            // 假设 Game 类中存在方法: bool tryMoveLeft();
            if (context.game.tryMoveLeft()) {
                syncBoardAndActionToUi();
            }
            break;

        case Qt::Key_Right: // 右移
        case Qt::Key_D:
            // 假设 Game 类中存在方法: bool tryMoveRight();
            if (context.game.tryMoveRight()) {
                syncBoardAndActionToUi();
            }
            break;

        case Qt::Key_Up: // 旋转
        case Qt::Key_W:
            // 假设 Game 类中存在方法: bool tryRotate();
            if (context.game.tryRotate()) {
                syncBoardAndActionToUi();
            }
            break;

        case Qt::Key_Down: // 硬着陆
        case Qt::Key_S:
        case Qt::Key_Space:
    while (context.game.moveDown()) {
        // 空循环，持续下落
    }
    context.game.placeCurrentBlock();
    context.game.clearFullRows();
    // 更新分数显示
    setScoreWidgetNumber(abs(context.game.score));
    context.game.spawnNewBlock();
    // 更新下一个方块显示
    setNextBlockWidget(context.game.next_block->color);
    
    if (context.game.isGameOver()) {
        context.status = GAME_OVER;
        timer.stop();
    }
    syncBoardAndActionToUi();
    break;

        default: // 其他
            QWidget::keyPressEvent(event);
            return;
        }
        break;

    case PAUSE: // 暂停状态
        if (event->key() == Qt::Key_P || event->key() == Qt::Key_Return ||
            event->key() == Qt::Key_Enter) // 'P'键或回车键取消暂停
        {
            context.status = PLAYING;
            timer.start(GAME_TIMER_INTERVAL_MS);
            syncBoardAndActionToUi();
        }
        break;

    case GAME_OVER: // 游戏结束状态
        if (event->key() == Qt::Key_Return ||
            event->key() == Qt::Key_Enter) // 回车键返回主菜单
        {
            context = Context();
            context.status = MAIN_MENU;
            // 计时器此时已停止
            syncBoardAndActionToUi();
        }
        break;

    case SETTINGS_MENU:
        // TODO: 实现完整的设置菜单按键绑定
        // 添加一个基础的 'Esc' 键返回主菜单的功能
        if (event->key() == Qt::Key_Escape) {
            context.status = MAIN_MENU;
            syncBoardAndActionToUi();
        } else {
            QWidget::keyPressEvent(event); // 其他
        }
        break;

    default: // 其他未知状态
        QWidget::keyPressEvent(event); 
        break;
    }
}

void Ui::MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // 目前，俄罗斯方块游戏不需要在按键释放时执行特定操作
    QWidget::keyReleaseEvent(event);
}
