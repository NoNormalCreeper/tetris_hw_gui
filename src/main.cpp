#include <QApplication>

#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Ui::MainWindow mainWindow;

    mainWindow.setWindowTitle("Tetris Game");

    mainWindow.show();

    return QApplication::exec();
}
