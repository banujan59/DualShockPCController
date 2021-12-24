#include "MainWindow.h"

MainWindow::MainWindow(DualShockController* pDualShockController, QWidget* parent) :
	m_pDualShockController(pDualShockController),
	QMainWindow(parent),
	m_testMouseSensitivityThread(nullptr)
{
	// TODO handle m_pDualShockController nullptr
	// UI setup
    ui.setupUi(this);
	ui.mouseSensitivitySlider->setRange(DualShockController::GetMinMouseSensitivityFactor(),
		DualShockController::GetMaxMouseSensitivityFactor());

	connect(ui.mouseSensitivitySpinBox, &QSpinBox::valueChanged, 
		[&](const int& newValue) {ui.mouseSensitivitySlider->setValue(newValue); });

	connect(ui.mouseSensitivitySlider, &QSlider::valueChanged,
		[&](const int& newValue) {ui.mouseSensitivitySpinBox->setValue(newValue); });
	
	connect(ui.mouseSensitivitySlider, &QSlider::valueChanged, this, &MainWindow::MouseSensitivityChanged);
	connect(ui.testMouseSensitivityButton, &QPushButton::clicked, this, &MainWindow::TestMouseSensitivityFor5Seconds);
	connect(ui.applyMouseSensitivity, &QPushButton::clicked, 
		[&]() {m_pDualShockController->SetMouseAccelerationFactor(ui.mouseSensitivitySpinBox->value()); MouseSensitivityChanged(ui.mouseSensitivitySpinBox->value()); });
	connect(this, &MainWindow::updateTestButtonTextSignal,
		[&](const QString& buttonText) { ui.testMouseSensitivityButton->setText(buttonText); });

	connect(this, &MainWindow::TestMouseSensitivityDoneSignal, this, &MainWindow::TestMouseSensitivityDoneSlot);

	ui.mouseSensitivitySpinBox->setValue(m_pDualShockController->GetMouseAccelerationFactor());

	// Display UI
	SetWidgetEnableState(true);
    show();

	// Attempt connection to DS
	if(m_pDualShockController->ConnectToDevice())
	{
		SetWidgetEnableState(false);
	}
}

void MainWindow::MouseSensitivityChanged(int newValue)
{
	if(newValue == m_pDualShockController->GetMouseAccelerationFactor())
	{
		ui.testMouseSensitivityButton->setDisabled(true);
		ui.applyMouseSensitivity->setDisabled(true);
	}

	else
	{
		ui.testMouseSensitivityButton->setDisabled(false);
		ui.applyMouseSensitivity->setDisabled(false);
	}
}

void MainWindow::TestMouseSensitivityFor5Seconds()
{
	ui.applyMouseSensitivity->setDisabled(true);
	ui.mouseSensitivitySpinBox->setDisabled(true);
	ui.mouseSensitivitySlider->setDisabled(true);

	int currentValue = m_pDualShockController->GetMouseAccelerationFactor();
	int newValue = ui.mouseSensitivitySpinBox->value();

	m_testMouseSensitivityThread.reset(new std::thread([&](const int& valueToTest, const int& originalValue)
	{
		m_pDualShockController->SetMouseAccelerationFactor(valueToTest);

		for (int i = 0; i < 5; i++)
		{
			QString buttonString = QString("Testing (%1s)").arg((5 - i));
			emit updateTestButtonTextSignal(buttonString);

			std::this_thread::sleep_for(std::chrono::seconds(1));
		}
		m_pDualShockController->SetMouseAccelerationFactor(originalValue);

		emit TestMouseSensitivityDoneSignal();	
	}, newValue, currentValue));
}

void MainWindow::TestMouseSensitivityDoneSlot()
{
	ui.applyMouseSensitivity->setDisabled(false);
	ui.mouseSensitivitySpinBox->setDisabled(false);
	ui.mouseSensitivitySlider->setDisabled(false);
	ui.testMouseSensitivityButton->setText("Test for 5 seconds");
	m_testMouseSensitivityThread->detach();
}

void MainWindow::SetWidgetEnableState(bool disableState)
{
	if(disableState)
	{
		ui.connectedStatusLabel->setText("No DualShock connected!!");
		ui.attemptConnectionButton->show();

		ui.testMouseSensitivityButton->setDisabled(true);
		ui.applyMouseSensitivity->setDisabled(true);
	}

	else
	{
		ui.connectedStatusLabel->setText("Connected to DualShock 4 device!");
		ui.attemptConnectionButton->hide();
		MouseSensitivityChanged(ui.mouseSensitivitySpinBox->value());
	}

	ui.mouseSensitivitySpinBox->setDisabled(disableState);
	ui.mouseSensitivitySlider->setDisabled(disableState);
}