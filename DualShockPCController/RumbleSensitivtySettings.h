#pragma once

#include <thread>
#include <QWidget>
#include "ui_MouseSensitivitySettings.h"
#include "DualShockController.h"

class RumbleSensitivtySettings : public QWidget
{
	Q_OBJECT

public:
	RumbleSensitivtySettings(DualShockController* pDualShockController, QWidget* parent = Q_NULLPTR);
	~RumbleSensitivtySettings();

private slots:
	void HandleRumbleSensitivityApplyButtonClicked();
	void HandleRumbleSensitivitySliderValueChanged(int newValue);
	void HandleTestRumbleSensitivityClicked();

public slots:
	void HandleWidgetEnableStateChange(bool disableState);

private:
	Ui::MouseSensitivitySettings ui;
	DualShockController* m_pDualShockController;
};
