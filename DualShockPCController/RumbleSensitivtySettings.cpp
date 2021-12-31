#include "RumbleSensitivtySettings.h"

RumbleSensitivtySettings::RumbleSensitivtySettings(DualShockController* pDualShockController, QWidget* parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController)
{
	if (m_pDualShockController == nullptr)
		return;

	ui.setupUi(this);

	connect(ui.sensitivitySpinBox, &QSpinBox::valueChanged,
		[&](const int& newValue) {ui.sensitivitySlider->setValue(newValue); });

	connect(ui.sensitivitySlider, &QSlider::valueChanged,
		[&](const int& newValue) {ui.sensitivitySpinBox->setValue(newValue); });

	connect(ui.sensitivitySlider, &QSlider::valueChanged, this, &RumbleSensitivtySettings::HandleRumbleSensitivitySliderValueChanged);
	connect(ui.testSensitivityButton, &QPushButton::clicked, this, &RumbleSensitivtySettings::HandleTestRumbleSensitivityClicked);
	connect(ui.applySensitivity, &QPushButton::clicked, this, &RumbleSensitivtySettings::HandleRumbleSensitivityApplyButtonClicked);

	connect(ui.resetToDefaultButton, &QPushButton::clicked, [&]()
		{
			ui.sensitivitySlider->setValue(m_pDualShockController->GetCurrentRumbleSensitivity());
		});

	ui.sensitivitySpinBox->setRange(DualShockController::MIN_RUMBLE_SENSITIVITY, DualShockController::MAX_RUMBLE_SENSITIVITY);
	ui.sensitivitySlider->setRange(DualShockController::MIN_RUMBLE_SENSITIVITY, DualShockController::MAX_RUMBLE_SENSITIVITY);
	ui.sensitivitySpinBox->setValue(m_pDualShockController->GetCurrentRumbleSensitivity());

	ui.sensitivityLabel->setText("Haptic feedback\nsensitivity:");
	ui.testSensitivityButton->setText("Test");
}

RumbleSensitivtySettings::~RumbleSensitivtySettings()
{
}

void RumbleSensitivtySettings::HandleWidgetEnableStateChange(bool disableState)
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
		HandleRumbleSensitivitySliderValueChanged(ui.sensitivitySpinBox->value());
	}

	ui.sensitivitySpinBox->setDisabled(disableState);
	ui.sensitivitySlider->setDisabled(disableState);
}

void RumbleSensitivtySettings::HandleRumbleSensitivityApplyButtonClicked()
{
	m_pDualShockController->SetRumbleSensitivity(ui.sensitivitySpinBox->value());
	HandleRumbleSensitivitySliderValueChanged(ui.sensitivitySpinBox->value());
}

void RumbleSensitivtySettings::HandleRumbleSensitivitySliderValueChanged(int newValue)
{
	if (newValue == m_pDualShockController->GetCurrentRumbleSensitivity())
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

void RumbleSensitivtySettings::HandleTestRumbleSensitivityClicked() const
{
	ui.testSensitivityButton->setDisabled(true);
	int rumbleValue = ui.sensitivitySpinBox->value();
	m_pDualShockController->TestRumble(rumbleValue);
	ui.testSensitivityButton->setDisabled(false);
}
