//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QLCDNumber>

#include <tuple>

static auto to_top = [](const auto y) -> auto {
    return Game::game_height - y - 1;
};

int Ui::MainWindow::getFrameId(const Pos& position) {
    // TODO: 建议修改为预处理存储
    const auto to_top_border = to_top(position.y);
    const auto to_left_border = position.x + 1;
    return 10 * to_top_border + to_left_border;
}

QFrame* Ui::MainWindow::getCell(const Pos& position) const {
    const auto object_name =
        QString("frame_%1").arg(QString::number(getFrameId(position)));
    const auto horizon_name =
        QString("horizontalLayout_%1").arg(QString::number(position.y + 1));

    const auto horizon = qobject_cast<QHBoxLayout*>(
        ui->gridArea->findChild<QHBoxLayout*>(horizon_name));
    if (!horizon) {
        qWarning("Horizon layout '%s' not found", qPrintable(horizon_name));
        return nullptr;
    }

    // In fact im not fully understand...
    for (int i = 0; i < horizon->count(); ++i) {
        if (const auto widget = horizon->itemAt(i)->widget();
            widget && widget->objectName() == object_name) {
            if (const auto frame = qobject_cast<QFrame*>(widget); frame) {
                return frame;
            }
        }
    }

    qWarning(
        "Frame '%s' not found as an item in layout '%s'. Layout item count: %d",
        qPrintable(object_name), qPrintable(horizon_name), horizon->count());
    return nullptr;
}

auto bgColorStylesheet(const QString& file_path) {
    // example: "cells/cells/red_cell.png"
    return QString("border-image: url(:/%1)").arg(file_path);
}

void Ui::MainWindow::setCellColor(const Pos& position,
                                  const std::optional<QString>& color) {
    const auto frame = getCell(position);
    if (frame) {
        auto stylesheet = QString("");
        if (color.has_value()) {
            // 若传入 color 为空，则重置为默认背景
            stylesheet = bgColorStylesheet(
                QString("cells/cells/%1_cell.png").arg(color.value()));
        }
        frame->setStyleSheet(stylesheet);
        return;
    }
    throw std::runtime_error(QString("Cell frame '%1' not found")
                                 .arg(QString::number(getFrameId(position)))
                                 .toStdString());
}

void Ui::MainWindow::setNextBlockWidget(const QString& color) {
    const auto nextBlockWidget = qobject_cast<QWidget*>(ui->nextBlockWidget);
    if (nextBlockWidget) {
        nextBlockWidget->setStyleSheet(bgColorStylesheet(
            QString("blocks/blocks/%1_block.png").arg(color)));
        return;
    }
    throw std::runtime_error("Next block widget not found");
}

void Ui::MainWindow::setScoreWidgetNumber(const int score) const {
    const auto scoreWidget = qobject_cast<QLCDNumber*>(ui->scoreNumber);
    if (scoreWidget) {
        scoreWidget->display(score);
        return;
    }
    throw std::runtime_error("Score widget not found");
}

void Ui::MainWindow::drawBlockOnBoard(const Block& block, const Pos& anchor) {
    for (const auto& pos : block.occupied) {
        const auto cell_pos = anchor + (pos - block.anchor);
        setCellColor(cell_pos, std::optional(block.color));
    }
}

void Ui::MainWindow::eraseBlockFromBoard(const Block& block,
                                         const Pos& anchor) {
    for (const auto& pos : block.occupied) {
        const auto cell_pos = anchor + (pos - block.anchor);
        setCellColor(cell_pos, std::nullopt);
    }
}

void Ui::MainWindow::moveBlock(const Block& block, const Pos& anchor,
                               const Pos& new_anchor) {
    eraseBlockFromBoard(block, anchor);
    drawBlockOnBoard(block, new_anchor);
}

void Ui::MainWindow::syncBoardAndActionToUi() {
    const auto& game = this->context.game;
    // draw game board
    for (int x = 0; x < game.game_width; ++x) {
        for (int y = 0; y < game.game_height; ++y) {
            const auto cell_pos = Pos(x, y);
            const auto& cell_content = game.game_board.at(y).at(x);

            if (!cell_content.has_value()) {
                setCellColor(cell_pos, std::nullopt);
            } else {
                const auto& cell = Block::getBlockByLabel(cell_content.value());
                setCellColor(cell_pos, std::optional(cell.color));
            }

        }
    }

    if (context.status != MAIN_MENU) {
        // 只有游戏状态为PLAYING时，才绘制当前活动方块，否则在开始菜单上显示新方块不太美观
        const auto& block = game.current_action.block;
        const auto& anchor = game.current_action.anchor;
        for (const auto& cell : block->occupied) {
            const auto cell_pos = anchor + (cell - block->anchor);
            setCellColor(cell_pos, std::optional(block->color));
        }
    } else {
        // 在非PLAYING状态时，清除当前方块占的格子颜色，防止残留显示
        const auto& block = game.current_action.block;
        const auto& anchor = game.current_action.anchor;
        for (const auto& cell : block->occupied) {
            const auto cell_pos = anchor + (cell - block->anchor);
            setCellColor(cell_pos, std::nullopt);
        }
    }

    toogleEndMenu(this->context.status);
}

void Ui::MainWindow::toogleStartMenu(int status) {
    auto start_menu_widget = ui->startMenu;
    if (start_menu_widget) {
        start_menu_widget->setVisible(status);
    } else {
        throw std::runtime_error("Start menu widget not found");
    }
}

void Ui::MainWindow::toogleEndMenu(int status) {
    if (!ui->endMenu) {
        throw std::runtime_error("End menu widget not found");
    }

    if (status == GAME_OVER) {
        ui->endMenu->setVisible(true);
        // 居中部分
        ui->endMenu->adjustSize();
        int px = (this->width() - ui->endMenu->width()) / 2;
        int py = (this->height() - ui->endMenu->height()) / 2;
        ui->endMenu->move(px, py);
        ui->endMenu->setVisible(true);

        // 显示本局分数
        ui->labelScoreInfo->setText(QString("本局得分: %1，真是太厉害啦！！！").arg(abs(context.game.score)));

        // 停止计时器
        timer.stop();

    } else {
        ui->endMenu->setVisible(false);
    }
}



void Ui::MainWindow::syncMenuStatusToUi() {
    // 控制主菜单（startMenu）
    toogleStartMenu(this->context.status == MAIN_MENU);

    // 控制结束菜单（endMenu）
    toogleEndMenu(this->context.status);

    // 只在暂停状态显示pauseMenu
    if(ui->pauseMenu){
        ui->pauseMenu->setVisible(this->context.status == PAUSE);
        if (this->context.status == PAUSE)
            ui->pauseMenu->raise(); // 可以让暂停菜单浮到最前
    }
}
