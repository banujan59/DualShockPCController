#include "MainWindow.h"

MainWindow::MainWindow(DualShockController* pDualShockController, QWidget* parent) :
	m_pDualShockController(pDualShockController),
	QMainWindow(parent)
{
    ui.setupUi(this);
    show();

	if(m_pDualShockController->ConnectToDevice())
	{
		ui.connectedStatusLabel->setText("Connected to DualShock 4 device!");
	}
}
