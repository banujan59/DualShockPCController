#include "MainWindow.h"

MainWindow::MainWindow(DualShockController& oDualShockController, QWidget* parent) :
	m_oDualShockController(oDualShockController),
	QMainWindow(parent)
{
    ui.setupUi(this);
    show();

	if(m_oDualShockController.ConnectToDevice())
	{
		ui.connectedStatusLabel->setText("Connected to DualShock 4 device!");
	}
}
