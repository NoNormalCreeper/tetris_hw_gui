//
// Created by Rikka on 2025/5/16.
//

#include "Game.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <tuple>


static auto to_top = [](const auto y) -> auto { return Game::game_height - y - 1; };

int Ui::MainWindow::getFrameId(const std::tuple<int, int> &position) {
    const auto to_top_border = to_top(std::get<1>(position));
    const auto to_left_border = std::get<0>(position) + 1;
    return 10 * to_top_border + to_left_border;
}

QFrame* Ui::MainWindow::getCell(const std::tuple<int, int> &position) const {
    const auto object_name = QString("frame_%1").arg(QString::number(getFrameId(position)));
    const auto horizon_name = QString("horizontalLayout_%1").arg(QString::number(std::get<1>(position) + 1));

    const auto horizon = qobject_cast<QHBoxLayout*>(ui->gridArea->findChild<QHBoxLayout*>(horizon_name));
    if (!horizon) {
        qWarning("Horizon layout '%s' not found", qPrintable(horizon_name));
        return nullptr;
    }

    // In fact im not fully understand...
    for (int i = 0; i < horizon->count(); ++i) {
        if (const auto widget = horizon->itemAt(i)->widget(); widget && widget->objectName() == object_name) {
            if (auto frame = qobject_cast<QFrame*>(widget); frame) {
                return frame;
            }
        }
    }

    qWarning("Frame '%s' not found as an item in layout '%s'. Layout item count: %d", qPrintable(object_name), qPrintable(horizon_name), horizon->count());
    return nullptr;
}

auto bgColorStylesheet(const QString& color) {
    return QString("border-image: url(:/cells/cells/%1_cell.png)").arg(color);
}

void Ui::MainWindow::setCellColor(const std::tuple<int, int> &position, const QString &color) {
    const auto frame = getCell(position);
    if (frame) {
        frame->setStyleSheet(bgColorStylesheet(color));
        return;
    }
    throw std::runtime_error("Frame to set color not found");
}
