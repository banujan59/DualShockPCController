#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

#include "DualShockController.h"
#include "ButtonLayoutSettings.h"
#include "GyroMouseControlSettings.h"
#include "MouseSensitivitySettings.h"
#include "RumbleSensitivtySettings.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);

signals:
    void WidgetEnableStateChangeSignal(bool disableState);

private slots:
    void HandleWidgetEnableStateChange(bool disableState);

private:
    Ui::MainWindowClass ui;
	DualShockController* m_pDualShockController;

    ButtonLayoutSettings m_buttonLayoutSettings;
    GyroMouseControlSettings m_gyroMouseControlSettings;
    MouseSensitivitySettings m_mouseSensitivitySettings;
    RumbleSensitivtySettings m_rumbleSensitivitySettings;
};
