#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Ui::MainWindow mainWindow;

    mainWindow.setWindowTitle("Test 1.0.0");

    mainWindow.show();

    return QApplication::exec();
}
