#pragma once

#include <thread>
#include <QWidget>
#include "ui_SensitivitySettings.h"
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
	void HandleTestRumbleSensitivityClicked() const;

public slots:
	void HandleWidgetEnableStateChange(bool disableState);

private:
	Ui::SensitivitySettings ui;
	DualShockController* m_pDualShockController;
};
