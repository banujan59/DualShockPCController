#pragma once

#include <QWidget>
#include "ui_LightBarColorSettings.h"
#include "DualShockController.h"

class LightBarColorSettings : public QWidget
{
	Q_OBJECT

public:
	LightBarColorSettings(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);
	~LightBarColorSettings();

public slots:
	void HandleWidgetEnableStateChange(bool disableState) const;

private slots:
	void HandleRandomColorRadioButtonToggled(bool checked);
	void HandleChooseColorButtonClicked();

private:
	Ui::LightBarColorSettings ui;
	DualShockController* m_pDualShockController;
	uint8_t m_redValue;
	uint8_t m_greenValue;
	uint8_t m_blueValue;
	LightBarMode m_lightBarMode;
	bool m_fadeColorEnabled;

	void UpdateColorPreview() const;
};
