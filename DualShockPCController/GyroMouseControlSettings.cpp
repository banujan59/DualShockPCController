#include "GyroMouseControlSettings.h"

GyroMouseControlSettings::GyroMouseControlSettings(DualShockController* pDualShockController, QWidget* parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController)
{

	if (m_pDualShockController == nullptr)
		return;

	ui.setupUi(this);
	ui.gyroControlStateCheckBox->setChecked(m_pDualShockController->IsGyroControlledMouseEnabled());
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

void GyroMouseControlSettings::HandleWidgetEnableStateChange(bool disableState)
{
	ui.gyroControlStateCheckBox->setDisabled(disableState);
	ui.gyroControlText->setDisabled(disableState);
}