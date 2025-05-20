//
// Created by Rikka on 2025/5/15.
//

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFrame>
#include <QWidget>

#include "Block.h"
#include "Pos.h"

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
    // void onButtonClicked(); // 按钮点击事件处理函数

    void TestCellDrawing(); // 测试单元格绘制函数
    void TestNextBlockDrawing();
    void TestDigitNumber();
    void TestBlockDrawing();


private:
    Ui::MainWindow *ui;

    static int getFrameId(const Pos &position);
    QFrame* getCell(const Pos &position) const;
    void setCellColor(const Pos &position, const QString &color);
    void setNextBlockWidget(const QString &color);
    void setScoreWidgetNumber(int score);

    void drawBlockOnBoard(const Block &block, const Pos &anchor);
    void eraseBlockFromBoard(const Block &block, const Pos &anchor);
    void moveBlock(const Block& block, const Pos& anchor, const Pos& new_anchor);
};
} // Ui

#endif //MAINWINDOW_H
