#include "MainWindow.h"

MainWindow::MainWindow(DualShockController* pDualShockController, QWidget* parent) :
	QMainWindow(parent),
	m_pDualShockController(pDualShockController),
	m_mouseSensitivitySettings(MouseSensitivitySettings(pDualShockController)),
	m_gyroMouseControlSettings(GyroMouseControlSettings(pDualShockController))
{
	// TODO handle m_pDualShockController nullptr
	// UI setup
    ui.setupUi(this);

	ui.gridLayout->addWidget(&m_gyroMouseControlSettings, 1, 1, 2, 1);
	ui.gridLayout->addWidget(&m_mouseSensitivitySettings, 1, 2, 3, 2);

	// connect WidgetEnableStateChangeSignal signal
	connect(this, &MainWindow::WidgetEnableStateChangeSignal, this, &MainWindow::HandleWidgetEnableStateChange);
	connect(this, &MainWindow::WidgetEnableStateChangeSignal, &m_mouseSensitivitySettings, &MouseSensitivitySettings::HandleWidgetEnableStateChange);

	// Display UI
	emit WidgetEnableStateChangeSignal(true);
    show();

	// Attempt connection to DS
	if(m_pDualShockController->ConnectToDevice())
	{
		emit WidgetEnableStateChangeSignal(false);
	}
}

void MainWindow::HandleWidgetEnableStateChange(bool disableState)
{
	if(disableState)
	{
		ui.connectedStatusLabel->setText("No DualShock connected!!");
		ui.attemptConnectionButton->show();
	}

	else
	{
		ui.connectedStatusLabel->setText("Connected to DualShock 4 device!");
		ui.attemptConnectionButton->hide();
	}
}