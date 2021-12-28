#include "GyroMouseControlSettings.h"

GyroMouseControlSettings::GyroMouseControlSettings(DualShockController* pDualShockController, QWidget* parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController)
{
	ui.setupUi(this);
	connect(ui.gyroControlStateCheckBox, &QCheckBox::stateChanged, this, &GyroMouseControlSettings::UpdateGyroControlEnabled);
}

GyroMouseControlSettings::~GyroMouseControlSettings()
{
}

void GyroMouseControlSettings::UpdateGyroControlEnabled(int state)
{
	if(state == 0)
	{
		m_pDualShockController->EnableGryoControlledMouse(false);
	}

	else
	{
		m_pDualShockController->EnableGryoControlledMouse(true);
	}
}
