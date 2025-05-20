//
// Created by Rikka on 2025/5/15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

#include "Block.h"

namespace Ui {
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // tests
    TestCellDrawing();
    TestNextBlockDrawing();
    TestDigitNumber();
    TestBlockDrawing();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::TestCellDrawing() {
    setCellColor({0, 5}, "red");
}

void MainWindow::TestNextBlockDrawing() {
    setNextBlockWidget("blue");
}

void MainWindow::TestDigitNumber() {
    setScoreWidgetNumber(114514);
}

void MainWindow::TestBlockDrawing() {
    drawBlockOnBoard(k_Block::I, Pos(6, 3));
    drawBlockOnBoard(k_Block::I.rotate(), Pos(6, 12));
}

int MainWindow::addCount() {
    count++;
    return count;
}
} // Ui
