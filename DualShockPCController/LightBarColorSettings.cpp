#include "LightBarColorSettings.h"
#include <QColorDialog>

LightBarColorSettings::LightBarColorSettings(DualShockController* pDualShockController, QWidget *parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController)
{
	if (m_pDualShockController == nullptr)
		return;
	
	ui.setupUi(this);

	m_pDualShockController->GetRGBLightBarColor(m_redValue, m_greenValue, m_blueValue);
	connect(ui.chooseColorButton, &QPushButton::clicked, this, &LightBarColorSettings::HandleChooseColorButtonClicked);
	connect(ui.fadeColorCheckBox, &QCheckBox::stateChanged, [&](const int& value)
		{
			if (value == 0)
				m_fadeColor = false;

			else
				m_fadeColor = true;
		}
	);

	UpdateColorPreview();
}

LightBarColorSettings::~LightBarColorSettings()
{
}


void LightBarColorSettings::HandleChooseColorButtonClicked()
{
	QColorDialog colorDialog(QColor(m_redValue, m_greenValue, m_blueValue), this);
	int returnVal = colorDialog.exec();

	if(returnVal == QColorDialog::Accepted)
	{
		m_redValue = static_cast<uint8_t>(colorDialog.currentColor().red());
		m_greenValue = static_cast<uint8_t>(colorDialog.currentColor().green());
		m_blueValue = static_cast<uint8_t>(colorDialog.currentColor().blue());
		UpdateColorPreview();
	}
}

void LightBarColorSettings::HandleWidgetEnableStateChange(bool disableState)
{
	
}

void LightBarColorSettings::UpdateColorPreview()
{
	QString red = QString::number(m_redValue);
	QString green = QString::number(m_greenValue);
	QString blue = QString::number(m_blueValue);
	QString style = QString("background-color: rgb(%1,%2,%3); border: 0px ;").arg(red, green, blue);
	ui.colorDisplayWidget->setStyleSheet(style);
}