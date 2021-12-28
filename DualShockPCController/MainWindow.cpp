#include "MainWindow.h"
#include <QtWidgets/QMessageBox>

MainWindow::MainWindow(DualShockController* pDualShockController, QWidget* parent) :
	QMainWindow(parent),
	m_pDualShockController(pDualShockController),
	m_mouseSensitivitySettings(MouseSensitivitySettings(pDualShockController)),
	m_gyroMouseControlSettings(GyroMouseControlSettings(pDualShockController))
{
	if(m_pDualShockController == nullptr)
	{
		QMessageBox msgBox;
		msgBox.setText("The application encoutered an internal error and must terminate now.\nTry contacting the author of the application.");
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.exec();

		exit(EXIT_FAILURE);
	}

	else
	{
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
		if (m_pDualShockController->ConnectToDevice())
		{
			emit WidgetEnableStateChangeSignal(false);
		}
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