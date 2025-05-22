//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tuple>


static auto to_top = [](const auto y) -> auto { return Game::game_height - y - 1; };

int Ui::MainWindow::getFrameId(const Pos &position) {   // TODO: 建议修改为预处理存储
    const auto to_top_border = to_top(position.y);
    const auto to_left_border = position.x + 1;
    return 10 * to_top_border + to_left_border;
}

QFrame* Ui::MainWindow::getCell(const Pos &position) const {
    const auto object_name = QString("frame_%1").arg(QString::number(getFrameId(position)));
    const auto horizon_name = QString("horizontalLayout_%1").arg(QString::number(position.y + 1));

    const auto horizon = qobject_cast<QHBoxLayout*>(ui->gridArea->findChild<QHBoxLayout*>(horizon_name));
    if (!horizon) {
        qWarning("Horizon layout '%s' not found", qPrintable(horizon_name));
        return nullptr;
    }

    // In fact im not fully understand...
    for (int i = 0; i < horizon->count(); ++i) {
        if (const auto widget = horizon->itemAt(i)->widget(); widget && widget->objectName() == object_name) {
            if (const auto frame = qobject_cast<QFrame*>(widget); frame) {
                return frame;
            }
        }
    }

    qWarning("Frame '%s' not found as an item in layout '%s'. Layout item count: %d", qPrintable(object_name), qPrintable(horizon_name), horizon->count());
    return nullptr;
}

auto bgColorStylesheet(const QString& file_path) {
    // example: "cells/cells/red_cell.png"
    return QString("border-image: url(:/%1)").arg(file_path);
}

void Ui::MainWindow::setCellColor(const Pos &position, const std::optional<QString> &color) {
    const auto frame = getCell(position);
    if (frame) {
        auto stylesheet = QString("");
        if (color.has_value()) {  // 若传入 color 为空，则重置为默认背景
            stylesheet = bgColorStylesheet(QString("cells/cells/%1_cell.png").arg(color.value()));
        }
        frame->setStyleSheet(stylesheet);
        return;
    }
    throw std::runtime_error(QString("Cell frame '%1' not found").arg(QString::number(getFrameId(position))).toStdString());
}

void Ui::MainWindow::setNextBlockWidget(const QString &color) {
    const auto nextBlockWidget = qobject_cast<QWidget*>(ui->nextBlockWidget);
    if (nextBlockWidget) {
        nextBlockWidget->setStyleSheet(bgColorStylesheet(QString("blocks/blocks/%1_block.png").arg(color)));
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

void Ui::MainWindow::drawBlockOnBoard(const Block &block, const Pos &anchor) {
    for (const auto &pos : block.occupied) {
        const auto cell_pos = anchor + (pos - block.anchor);
        setCellColor(cell_pos, std::optional(block.color));
    }
}

void Ui::MainWindow::eraseBlockFromBoard(const Block &block, const Pos &anchor) {
    for (const auto &pos : block.occupied) {
        const auto cell_pos = anchor + (pos - block.anchor);
        setCellColor(cell_pos, std::nullopt);
    }
}

void Ui::MainWindow::moveBlock(const Block &block, const Pos &anchor, const Pos &new_anchor) {
    eraseBlockFromBoard(block, anchor);
    drawBlockOnBoard(block, new_anchor);
}

void Ui::MainWindow::syncBoardToUi(const Context &ctx) {
    const auto& game = ctx.game;
    for (int x = 0; x < game.game_width; ++x) {
        for (int y = 0; y < game.game_height; ++y) {
            const auto cell_pos = Pos(x, y);
            const auto& cell_content = game.game_board.at(y).at(x);

            if (!cell_content.has_value()) {
                setCellColor(cell_pos, std::nullopt);
                return;
            }
            const auto& cell = Block::getBlockByLabel(cell_content.value());
            setCellColor(cell_pos, std::optional(cell.color));
        }
    }
}
