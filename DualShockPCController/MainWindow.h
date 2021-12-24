#pragma once

#include <thread>
#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "DualShockController.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);

private slots:
    void MouseSensitivityChanged(int newValue);
    void TestMouseSensitivityFor5Seconds();
    void TestMouseSensitivityDoneSlot();

signals:
    void updateTestButtonTextSignal(QString text);
	void TestMouseSensitivityDoneSignal();

private:
    Ui::MainWindowClass ui;
    DualShockController* m_pDualShockController;
    std::unique_ptr<std::thread> m_testMouseSensitivityThread;

    void SetWidgetEnableState(bool disableState);
};
