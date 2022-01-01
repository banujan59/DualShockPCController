#pragma once

#include <thread>
#include <QWidget>
#include "ui_MouseSensitivitySettings.h"
#include "DualShockController.h"

class MouseSensitivitySettings : public QWidget
{
	Q_OBJECT

public:
	MouseSensitivitySettings(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);
	~MouseSensitivitySettings();

private slots:
	void HandleMouseSensitivityApplyButtonClicked();
	void HandleMouseSensitivitySliderValueChanged(int newValue);
	void HandleTestMouseSensitivityFor5Seconds();
	void HandleTestMouseSensitivityDoneSlot();

public slots:
	void HandleWidgetEnableStateChange(bool disableState);

signals:
	void UpdateTestButtonTextSignal(QString text);
	void TestMouseSensitivityDoneSignal();

private:
	Ui::SensitivitySettings ui;
	DualShockController* m_pDualShockController;
	std::unique_ptr<std::thread> m_testMouseSensitivityThread;
};
