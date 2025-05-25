//
// Created by Rikka on 2025/5/22.
//

#include "mainwindow.h"
#include <QKeyEvent>

// 定义游戏速度，后续可以调整或设为动态变化，比如demo设计的是和等级挂钩
const int GAME_TIMER_INTERVAL_MS = 500; // 游戏计时器间隔，单位：毫秒

void Ui::MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (!event)
    {
        return;
    }

    switch (context.status)
    {
    case MAIN_MENU:
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {
            // 可能会不匹配，还需要再修改
            context.game = Game();
            context.status = PLAYING;
            timer.start(GAME_TIMER_INTERVAL_MS);
            syncBoardAndActionToUi();
        }
        break;

    case PLAYING:
        if (!context.game.current_action.block)
        { // 理论上不应发生
            return;
        }
        switch (event->key())
        {
        case Qt::Key_Left:
        case Qt::Key_A:
            // 假设 Game 类中有一个 tryMoveLeft()
            if (context.game.tryMoveLeft())
            {
                syncBoardAndActionToUi();
            }
            break;

        case Qt::Key_Right:
        case Qt::Key_D:
            // 假设 Game 类中有一个 tryMoveRight()
            if (context.game.tryMoveRight())
            {
                syncBoardAndActionToUi();
            }

            break;

        case Qt::Key_Up:
        case Qt::Key_W:
            // 假设 Game 类中有一个 tryRotate()
            if (context.game.tryRotate())
            {
                syncBoardAndActionToUi();
            }

            break;

        case Qt::Key_Down:
        case Qt::Key_S:
        case Qt::Key_Space:
            // 直接触底
            while (context.game.moveDown())
            {
                // 保持下落
            }
            context.game.lockCurrentBlock();
            context.game.clearFullRows();
            context.game.spawnNewBlock();
            if (context.game.isGameOver())
            {
                context.status = GAME_OVER;
                timer.stop();
            }
            syncBoardAndActionToUi();
            break;

        default:
            QWidget::keyPressEvent(event);
            return;
        }
        if (context.status == GAME_OVER)
        {
            syncBoardAndActionToUi();
        }
        break;

    case GAME_OVER:
        if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
        {

            context = Context();
            syncBoardAndActionToUi();
        }
        break;

    case SETTINGS_MENU:
        // TODO: 实现设置菜单的按键绑定
        break;

    default:
        QWidget::keyPressEvent(event);
        break;
    }
}

void Ui::MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    // 目前，没有在按键释放时执行的操作，好像对于俄罗斯方块确实不需要？
    QWidget::keyReleaseEvent(event);
}
