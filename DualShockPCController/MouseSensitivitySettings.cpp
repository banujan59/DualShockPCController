#include "MouseSensitivitySettings.h"

MouseSensitivitySettings::MouseSensitivitySettings(DualShockController* pDualShockController, QWidget *parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController),
	m_testMouseSensitivityThread(nullptr)
{
	if (m_pDualShockController == nullptr)
		return;

	ui.setupUi(this);

	connect(ui.sensitivitySpinBox, &QSpinBox::valueChanged,
		[&](const int& newValue) {ui.sensitivitySlider->setValue(newValue); });

	connect(ui.sensitivitySlider, &QSlider::valueChanged,
		[&](const int& newValue) {ui.sensitivitySpinBox->setValue(newValue); });

	connect(ui.sensitivitySlider, &QSlider::valueChanged, this, &MouseSensitivitySettings::HandleMouseSensitivitySliderValueChanged);
	connect(ui.testSensitivityButton, &QPushButton::clicked, this, &MouseSensitivitySettings::HandleTestMouseSensitivityFor5Seconds);

	connect(ui.resetToDefaultButton, &QPushButton::clicked, [&]()
		{
			ui.sensitivitySlider->setValue(m_pDualShockController->GetMouseAccelerationFactor());
		});

	connect(ui.applySensitivity, &QPushButton::clicked, this, &MouseSensitivitySettings::HandleMouseSensitivityApplyButtonClicked);
	connect(this, &MouseSensitivitySettings::UpdateTestButtonTextSignal,
		[&](const QString& buttonText) { ui.testSensitivityButton->setText(buttonText); });

	connect(this, &MouseSensitivitySettings::TestMouseSensitivityDoneSignal, this, &MouseSensitivitySettings::HandleTestMouseSensitivityDoneSlot);

	ui.sensitivitySpinBox->setRange(DualShockController::MIN_MOUSE_SENSITIVITY,
		DualShockController::MAX_MOUSE_SENSITIVITY);
	ui.sensitivitySlider->setRange(DualShockController::MIN_MOUSE_SENSITIVITY,
		DualShockController::MAX_MOUSE_SENSITIVITY);
	ui.sensitivitySpinBox->setValue(m_pDualShockController->GetMouseAccelerationFactor());
}

MouseSensitivitySettings::~MouseSensitivitySettings()
{
}

void MouseSensitivitySettings::HandleWidgetEnableStateChange(bool disableState)
{
	if (disableState)
	{
		ui.sensitivityLabel->setDisabled(true);
		ui.testSensitivityButton->setDisabled(true);
		ui.resetToDefaultButton->setDisabled(true);
		ui.applySensitivity->setDisabled(true);
	}

	else
	{
		ui.sensitivityLabel->setDisabled(false);
		HandleMouseSensitivitySliderValueChanged(ui.sensitivitySpinBox->value());
	}

	ui.sensitivitySpinBox->setDisabled(disableState);
	ui.sensitivitySlider->setDisabled(disableState);
}

void MouseSensitivitySettings::HandleMouseSensitivityApplyButtonClicked()
{
	m_pDualShockController->SetMouseAccelerationFactor(ui.sensitivitySpinBox->value());
	HandleMouseSensitivitySliderValueChanged(ui.sensitivitySpinBox->value());
}

void MouseSensitivitySettings::HandleMouseSensitivitySliderValueChanged(int newValue)
{
	if (newValue == m_pDualShockController->GetMouseAccelerationFactor())
	{
		ui.testSensitivityButton->setDisabled(true);
		ui.resetToDefaultButton->setDisabled(true);
		ui.applySensitivity->setDisabled(true);
	}

	else
	{
		ui.testSensitivityButton->setDisabled(false);
		ui.resetToDefaultButton->setDisabled(false);
		ui.applySensitivity->setDisabled(false);
	}
}

void MouseSensitivitySettings::HandleTestMouseSensitivityFor5Seconds()
{
	ui.applySensitivity->setDisabled(true);
	ui.sensitivitySpinBox->setDisabled(true);
	ui.sensitivitySlider->setDisabled(true);
	ui.testSensitivityButton->setDisabled(true);

	int currentValue = m_pDualShockController->GetMouseAccelerationFactor();
	int newValue = ui.sensitivitySpinBox->value();

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
	ui.applySensitivity->setDisabled(false);
	ui.sensitivitySpinBox->setDisabled(false);
	ui.sensitivitySlider->setDisabled(false);
	ui.testSensitivityButton->setDisabled(false);
	ui.testSensitivityButton->setText("Test for 5 seconds");
	m_testMouseSensitivityThread->detach();
}
