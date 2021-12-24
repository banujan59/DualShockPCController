#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DualShockController oDualShockController;
    MainWindow w(oDualShockController);
    return a.exec();
}
