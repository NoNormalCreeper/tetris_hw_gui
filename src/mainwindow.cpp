//
// Created by Rikka on 2025/5/15.
//

// You may need to build the project (run Qt uic code generator) to get
// "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"
// #include "loop.cpp"
#include "Block.h"
#include <QGraphicsDropShadowEffect>
#include "mainwindow.h"


namespace Ui {
MainWindow::MainWindow(QWidget* parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    //在游戏窗口内强制转换为深色模式，看起来更美观
    QString darkStyle = R"(
QWidget {
    background-color: #232323;
    color: #EEEEEE;
}
QFrame {
    background-color: #232323;
    color: #EEEEEE;
}
QPushButton {
    background-color: #444;
    color: #EEEEEE;
    border: 1px solid #555;
    border-radius: 3px;
    padding: 3px 8px;
}
QLabel {
    color: #EEEEEE;
}
QLCDNumber {
    background: #333;
    color: #80FF80;
    border: none;
}
)";

    // 应用到主窗口（包括其子控件）
    this->setStyleSheet(darkStyle);


    // this->context = Context();
    setNextBlockWidget(context.game.next_block->color);

    // connect timer timeout signal
    connect(&timer, &QTimer::timeout, this, [&]() {
        onTimeOut(this->context);
    });

    // tests
    // TestCellDrawing();
    // TestNextBlockDrawing();
    // TestDigitNumber();
    // TestBlockDrawing();

    //初始状态下隐藏死亡菜单和暂停菜单
    ui->endMenu->setVisible(false);
    ui->pauseMenu->setVisible(false);

    // “Game Over”标签阴影代码
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect(this);
    effect->setBlurRadius(16);
    effect->setOffset(0, 6);
    effect->setColor(QColor(0, 0, 0, 156));
    ui->labelGameOver->setGraphicsEffect(effect);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::TestCellDrawing() { setCellColor({0, 5}, "red"); }

void MainWindow::TestNextBlockDrawing() { setNextBlockWidget("blue"); }

void MainWindow::TestDigitNumber() { setScoreWidgetNumber(114514); }

void MainWindow::TestBlockDrawing() {
    drawBlockOnBoard(k_Block::I, Pos(6, 3));
    // drawBlockOnBoard(k_Block::I.rotate(), Pos(6, 12));
}
int MainWindow::addCount() {
    count++;
    return count;
}


} // Ui
