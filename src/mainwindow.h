//
// Created by Rikka on 2025/5/15.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

namespace Ui {
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QWidget {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    int count = 0;
    int addCount(); // 增加计数器，返回当前计数值
    void onButtonClicked(); // 按钮点击事件处理函数


private:
    Ui::MainWindow *ui;
};
} // Ui

#endif //MAINWINDOW_H
