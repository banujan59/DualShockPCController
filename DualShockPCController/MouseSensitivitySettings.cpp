#include "MouseSensitivitySettings.h"

MouseSensitivitySettings::MouseSensitivitySettings(DualShockController* pDualShockController, QWidget *parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController),
	m_testMouseSensitivityThread(nullptr)
{
	if (m_pDualShockController == nullptr)
		return;

	ui.setupUi(this);

	connect(ui.mouseSensitivitySpinBox, &QSpinBox::valueChanged,
		[&](const int& newValue) {ui.mouseSensitivitySlider->setValue(newValue); });

	connect(ui.mouseSensitivitySlider, &QSlider::valueChanged,
		[&](const int& newValue) {ui.mouseSensitivitySpinBox->setValue(newValue); });

	connect(ui.mouseSensitivitySlider, &QSlider::valueChanged, this, &MouseSensitivitySettings::HandleMouseSensitivitySliderValueChanged);
	connect(ui.testMouseSensitivityButton, &QPushButton::clicked, this, &MouseSensitivitySettings::HandleTestMouseSensitivityFor5Seconds);

	connect(ui.resetToDefaultButton, &QPushButton::clicked, [&]()
		{
			ui.mouseSensitivitySlider->setValue(m_pDualShockController->GetMouseAccelerationFactor());
		});

	connect(ui.applyMouseSensitivity, &QPushButton::clicked, this, &MouseSensitivitySettings::HandleMouseSensitivityApplyButtonClicked);
	connect(this, &MouseSensitivitySettings::UpdateTestButtonTextSignal,
		[&](const QString& buttonText) { ui.testMouseSensitivityButton->setText(buttonText); });

	connect(this, &MouseSensitivitySettings::TestMouseSensitivityDoneSignal, this, &MouseSensitivitySettings::HandleTestMouseSensitivityDoneSlot);

	ui.mouseSensitivitySpinBox->setRange(DualShockController::MIN_MOUSE_SENSITIVITY,
		DualShockController::MAX_MOUSE_SENSITIVITY);
	ui.mouseSensitivitySlider->setRange(DualShockController::MIN_MOUSE_SENSITIVITY,
		DualShockController::MAX_MOUSE_SENSITIVITY);
	ui.mouseSensitivitySpinBox->setValue(m_pDualShockController->GetMouseAccelerationFactor());
}

MouseSensitivitySettings::~MouseSensitivitySettings()
{
}

void MouseSensitivitySettings::HandleWidgetEnableStateChange(bool disableState)
{
	if (disableState)
	{
		ui.mouseSensitivityLabel->setDisabled(true);
		ui.testMouseSensitivityButton->setDisabled(true);
		ui.resetToDefaultButton->setDisabled(true);
		ui.applyMouseSensitivity->setDisabled(true);
	}

	else
	{
		ui.mouseSensitivityLabel->setDisabled(false);
		HandleMouseSensitivitySliderValueChanged(ui.mouseSensitivitySpinBox->value());
	}

	ui.mouseSensitivitySpinBox->setDisabled(disableState);
	ui.mouseSensitivitySlider->setDisabled(disableState);
}

void MouseSensitivitySettings::HandleMouseSensitivityApplyButtonClicked()
{
	m_pDualShockController->SetMouseAccelerationFactor(ui.mouseSensitivitySpinBox->value());
	HandleMouseSensitivitySliderValueChanged(ui.mouseSensitivitySpinBox->value());
}

void MouseSensitivitySettings::HandleMouseSensitivitySliderValueChanged(int newValue)
{
	if (newValue == m_pDualShockController->GetMouseAccelerationFactor())
	{
		ui.testMouseSensitivityButton->setDisabled(true);
		ui.resetToDefaultButton->setDisabled(true);
		ui.applyMouseSensitivity->setDisabled(true);
	}

	else
	{
		ui.testMouseSensitivityButton->setDisabled(false);
		ui.resetToDefaultButton->setDisabled(false);
		ui.applyMouseSensitivity->setDisabled(false);
	}
}

void MouseSensitivitySettings::HandleTestMouseSensitivityFor5Seconds()
{
	ui.applyMouseSensitivity->setDisabled(true);
	ui.mouseSensitivitySpinBox->setDisabled(true);
	ui.mouseSensitivitySlider->setDisabled(true);
	ui.testMouseSensitivityButton->setDisabled(true);

	int currentValue = m_pDualShockController->GetMouseAccelerationFactor();
	int newValue = ui.mouseSensitivitySpinBox->value();

	m_testMouseSensitivityThread.reset(new std::thread([&](const int& valueToTest, const int& originalValue)
		{
			m_pDualShockController->SetMouseAccelerationFactor(valueToTest);

			for (int i = 0; i < 5; i++)
			{
				const QString buttonString = QString("Testing (%1s)").arg((5 - i));
				emit UpdateTestButtonTextSignal(buttonString);

				std::this_thread::sleep_for(std::chrono::seconds(1));
			}

			m_pDualShockController->SetMouseAccelerationFactor(originalValue);

			emit TestMouseSensitivityDoneSignal();
		}, newValue, currentValue));
}

void MouseSensitivitySettings::HandleTestMouseSensitivityDoneSlot()
{
	ui.applyMouseSensitivity->setDisabled(false);
	ui.mouseSensitivitySpinBox->setDisabled(false);
	ui.mouseSensitivitySlider->setDisabled(false);
	ui.testMouseSensitivityButton->setDisabled(false);
	ui.testMouseSensitivityButton->setText("Test for 5 seconds");
	m_testMouseSensitivityThread->detach();
}
