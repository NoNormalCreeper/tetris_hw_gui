//
// Created by Rikka on 2025/5/15.
//

// You may need to build the project (run Qt uic code generator) to get "ui_MainWindow.h" resolved

#include "mainwindow.h"
#include "ui_MainWindow.h"

namespace Ui {
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // 设置按钮点击事件
    // connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

int MainWindow::addCount() {
    count++;
    return count;
}

void MainWindow::onButtonClicked() {
    const int currentCount = addCount();
    ui->label->setText(QString("Count: %1").arg(QString::number(currentCount)));
}
} // Ui
