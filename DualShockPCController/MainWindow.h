#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "DualShockController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);

private:
    Ui::MainWindowClass ui;
    DualShockController* m_pDualShockController;
};
