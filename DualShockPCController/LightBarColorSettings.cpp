#include "LightBarColorSettings.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QRadioButton>

LightBarColorSettings::LightBarColorSettings(DualShockController* pDualShockController, QWidget *parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController)
{
	if (m_pDualShockController == nullptr)
		return;
	
	ui.setupUi(this);
	
	m_pDualShockController->GetRGBLightBarColor(m_redValue, m_greenValue, m_blueValue);
	m_lightBarMode = m_pDualShockController->GetLightBarMode();
	m_fadeColorEnabled = m_pDualShockController->GetLightBarFadeEnabled();

	if (m_lightBarMode == LightBarMode::SINGLE_COLOR)
		ui.singleColorModeRadio->setChecked(true);
	else
		ui.randomColorRadio->setChecked(true);

	connect(ui.chooseColorButton, &QPushButton::clicked, this, &LightBarColorSettings::HandleChooseColorButtonClicked);
	connect(ui.randomColorRadio, &QRadioButton::toggled, this, &LightBarColorSettings::HandleRandomColorRadioButtonToggled);
	connect(ui.fadeColorCheckBox, &QCheckBox::stateChanged, [&](const int& value)
		{
			if (value == 0)
				m_fadeColorEnabled = false;

			else
				m_fadeColorEnabled = true;
		}
	);

	UpdateColorPreview();
}

LightBarColorSettings::~LightBarColorSettings()
{
}


void LightBarColorSettings::HandleWidgetEnableStateChange(bool disableState) const
{
	if(disableState)
	{
		ui.selectedColorLabel->setDisabled(true);
		ui.chooseColorButton->setDisabled(true);
		ui.previewSelectedColorLabel->setDisabled(true);
		ui.colorDisplayWidget->setDisabled(true);
		ui.fadeColorCheckBox->setDisabled(true);

		ui.singleColorModeRadio->setDisabled(true);
		ui.randomColorRadio->setDisabled(true);
	}


	else
	{
		ui.singleColorModeRadio->setDisabled(false);
		ui.randomColorRadio->setDisabled(false);

		if (m_lightBarMode == LightBarMode::RANDOM_COLOR)
		{
			ui.selectedColorLabel->setDisabled(true);
			ui.chooseColorButton->setDisabled(true);
			ui.previewSelectedColorLabel->setDisabled(true);
			ui.colorDisplayWidget->setDisabled(true);
			ui.fadeColorCheckBox->setDisabled(true);
		}

		else if (m_lightBarMode == LightBarMode::SINGLE_COLOR)
		{
			ui.selectedColorLabel->setDisabled(false);
			ui.chooseColorButton->setDisabled(false);
			ui.previewSelectedColorLabel->setDisabled(false);
			ui.colorDisplayWidget->setDisabled(false);
			ui.fadeColorCheckBox->setDisabled(false);
		}
	}
}

void LightBarColorSettings::HandleRandomColorRadioButtonToggled(bool checked)
{
	if(checked)
	{
		m_lightBarMode = LightBarMode::RANDOM_COLOR;
	}

	else
	{
		m_lightBarMode = LightBarMode::SINGLE_COLOR;
	}

	m_pDualShockController->SetLightBarMode(m_lightBarMode, m_fadeColorEnabled);
	HandleWidgetEnableStateChange(false);
}

void LightBarColorSettings::HandleChooseColorButtonClicked()
{
	QColorDialog colorDialog(QColor(m_redValue, m_greenValue, m_blueValue), this);
	const int returnVal = colorDialog.exec();

	if (returnVal == QColorDialog::Accepted)
	{
		m_redValue = static_cast<uint8_t>(colorDialog.currentColor().red());
		m_greenValue = static_cast<uint8_t>(colorDialog.currentColor().green());
		m_blueValue = static_cast<uint8_t>(colorDialog.currentColor().blue());

		m_pDualShockController->SetRGBLightBarColor(m_redValue, m_greenValue, m_blueValue);
		UpdateColorPreview();
	}
}

void LightBarColorSettings::UpdateColorPreview() const
{
	QString red = QString::number(m_redValue);
	QString green = QString::number(m_greenValue);
	QString blue = QString::number(m_blueValue);
	QString style = QString("background-color: rgb(%1,%2,%3); border: 0px ;").arg(red, green, blue);
	ui.colorDisplayWidget->setStyleSheet(style);
}
