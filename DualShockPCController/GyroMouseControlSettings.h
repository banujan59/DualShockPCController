#pragma once

#include <QWidget>

#include "DualShockController.h"
#include "ui_GyroMouseControlSettings.h"

class GyroMouseControlSettings : public QWidget
{
	Q_OBJECT

public:
	GyroMouseControlSettings(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);
	~GyroMouseControlSettings();

private slots:
	void UpdateGyroControlEnabled(int state);

public slots:
	void HandleWidgetEnableStateChange(bool disableState);

private:
	Ui::GyroMouseControlSettings ui;
	DualShockController* m_pDualShockController;
};
