#include "AddCustomCommandDialog.h"
#include <QFileDialog>
#include <QMessageBox>

AddCustomCommandDialog::AddCustomCommandDialog(DualShockController* pDualShockController, QWidget* parent)
	: QDialog(parent),
	m_pDualShockController(pDualShockController),
	m_pThread(nullptr)
{
	if (m_pDualShockController == nullptr)
		return;

	ui.setupUi(this);

	// populate action type
	DualShockController::GetCustomCommandsActions(m_actionTypeNames);
	ui.actionTypeSelector->addItem("Select Action");
	std::ranges::for_each(m_actionTypeNames, [&](const std::pair<CustomButtonSequence::ActionType, std::string>& actionName)
		{
			ui.actionTypeSelector->addItem(QString::fromStdString(actionName.second));
		}
	);

	connect(ui.actionTypeSelector, &QComboBox::currentIndexChanged, this, &AddCustomCommandDialog::HandleActionTypeChanged);

	// populate button selector
	DualShockController::GetDSButtonNames(m_dsButtonNames);
	std::ranges::for_each(m_dsButtonNames, [&](const std::pair<int, std::string>& actionName)
		{
			ui.buttonSelector->addItem(QString::fromStdString(actionName.second));
		}
	);
	ui.buttonSelector->setEditable(true);
	m_buttonSequenceListModel = new ButtonSequenceModel(this);
	ui.buttonSequenceList->setModel(m_buttonSequenceListModel);

	ui.addButtonWithDSInstructionLabel->hide();
	ui.addButtonWithDSTimerLabel->hide();

	connect(ui.addButtonToSequenceButton, &QPushButton::clicked, this, &AddCustomCommandDialog::HandleAddNewButtonSequence);
	connect(ui.addButtonToSequenceWithDS, &QPushButton::clicked, this, &AddCustomCommandDialog::HandleAddNewButtonSequenceWithDS);
	connect(this, &AddCustomCommandDialog::ButtonSequenceWithDSDoneSignal, this, &AddCustomCommandDialog::HandleAddNewButtonSequenceWithDSDone);
	connect(this, &AddCustomCommandDialog::UpdateDSButtonSequenceTimerSignal, this, &AddCustomCommandDialog::HandleAddNewButtonSequenceWithDSTimerUpdate);
	connect(this, &AddCustomCommandDialog::NewDSButtonSequenceEntered, this, &AddCustomCommandDialog::HandleNewDSButtonSequenceEntered);
	connect(ui.okButton, &QPushButton::clicked, this, &AddCustomCommandDialog::HandleOkButtonClicked);
}

AddCustomCommandDialog::~AddCustomCommandDialog()
{
}

void AddCustomCommandDialog::HandleActionTypeChanged(int index)
{
	index--; // first element of the selector must be ignored

	if(index >= 0 && index < m_actionTypeNames.size())
	{
		auto it = m_actionTypeNames.begin();
		std::advance(it, index);

		// TODO don't like how this class has direct access to CustomButtonSequence...
		if(it != m_actionTypeNames.end() && it->first == CustomButtonSequence::ActionType::OPEN_FILE_OR_PROGRAM)
		{
			m_actionType = CustomButtonSequence::ActionType::OPEN_FILE_OR_PROGRAM;
			m_actionTypeName = it->second;

			QStringList selectedFilenames = QFileDialog::getOpenFileNames(this,
				tr("Select program(s) or file(s) to open for this command"));

			if(!selectedFilenames.empty())
			{
				m_actionParameters.clear();
				for (QString qFilename : selectedFilenames)
				{
					m_actionParameters.push_back(qFilename.toStdString());
				}
			}
		}
	}
}

void AddCustomCommandDialog::HandleAddNewButtonSequence()
{
	const int index = ui.buttonSelector->currentIndex();
	if(index >= 0 && static_cast<unsigned int>(index) < m_dsButtonNames.size())
	{
		auto it = m_dsButtonNames.begin();
		std::advance(it, index);

		UpdateButtonSequenceList(it);
	}

	else if(index > 0) // because the field is editable, invalid data can be entered
	{
		ui.buttonSelector->removeItem(index);
	}

}

void AddCustomCommandDialog::HandleAddNewButtonSequenceWithDS()
{
	ui.buttonSelector->setDisabled(true);
	ui.addButtonToSequenceButton->setDisabled(true);
	ui.addButtonToSequenceWithDS->setDisabled(true);
	ui.cancelButton->setDisabled(true);
	ui.okButton->setDisabled(true);
	ui.addButtonWithDSInstructionLabel->show();


	m_pThread.reset(new std::thread([&]()
		{
			m_pDualShockController->SetDSButtonSequenceMode(true);
			std::chrono::time_point<std::chrono::steady_clock> timeWithNoButtonStart;

			bool timeElapsed = false;
			int previousIterationButtons = -1;
			while(!timeElapsed)
			{
				int buttons = m_pDualShockController->GetLatestButtonDown();
				if(m_dsButtonNames.count(buttons) == 0)
				{
					// if we are here, no buttons are pressed. we can start the countdown to end this thread.
					if(previousIterationButtons != buttons)
						timeWithNoButtonStart = std::chrono::steady_clock::now();

					else
					{
						auto end = std::chrono::steady_clock::now();
						int secondsElapsed = std::chrono::duration_cast<std::chrono::seconds>(end - timeWithNoButtonStart).count();

						if (secondsElapsed >= DS_ENTER_BUTTON_SEQUENCE_MAX_SECONDS)
							timeElapsed = true;
						else
						{
							emit UpdateDSButtonSequenceTimerSignal(secondsElapsed);
						}
					}
				}

				else if(previousIterationButtons != buttons)
				{
					emit NewDSButtonSequenceEntered(buttons);
				}

				previousIterationButtons = buttons;
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
			}

			m_pDualShockController->SetDSButtonSequenceMode(false);
			emit ButtonSequenceWithDSDoneSignal();
		})
	);
}

void AddCustomCommandDialog::HandleAddNewButtonSequenceWithDSTimerUpdate(int secondsElapsed) const
{
	QString text = QString("Ending in %1 seconds...").arg(DS_ENTER_BUTTON_SEQUENCE_MAX_SECONDS -secondsElapsed);
	ui.addButtonWithDSTimerLabel->setText(text);
	ui.addButtonWithDSTimerLabel->show();
}

void AddCustomCommandDialog::HandleNewDSButtonSequenceEntered(int buttons)
{
	ui.addButtonWithDSTimerLabel->hide();

	if(auto it = m_dsButtonNames.find(buttons) ; it != m_dsButtonNames.end())
	{
		UpdateButtonSequenceList(it);
	}
}

void AddCustomCommandDialog::HandleAddNewButtonSequenceWithDSDone() const
{
	ui.buttonSelector->setDisabled(false);
	ui.addButtonToSequenceButton->setDisabled(false);
	ui.addButtonToSequenceWithDS->setDisabled(false);
	ui.cancelButton->setDisabled(false);
	ui.okButton->setDisabled(false);
	ui.addButtonWithDSInstructionLabel->hide();
	ui.addButtonWithDSTimerLabel->hide();

	m_pThread->detach();
}

void AddCustomCommandDialog::HandleDeleteButtonSequence()
{
}

void AddCustomCommandDialog::HandleOkButtonClicked()
{
	if(ui.commandNameField->text().isEmpty())
	{
		QMessageBox msgBox;
		msgBox.setText("Please enter a name for this command.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}

	m_commandName = ui.commandNameField->text().toStdString();

	if(ui.actionTypeSelector->currentIndex() == 0)
	{
		QMessageBox msgBox;
		msgBox.setText("Please select an action type for this command.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}

	if(m_buttonSequence.size() <= 1)
	{
		QMessageBox msgBox;
		msgBox.setText("Please add at least 2 buttons for this command.");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.exec();
		return;
	}

	accept();
}

void AddCustomCommandDialog::UpdateButtonSequenceList(const std::map<int, std::string>::const_iterator& it)
{
	m_buttonSequence.push_back(it->first);
	m_buttonSequenceNames.push_back(it->second);
	m_buttonSequenceListModel->InsertRow(it->second);

	ui.buttonSequenceList->scrollToBottom();
}

std::string AddCustomCommandDialog::GetCommandName()
{
	return m_commandName;
}

std::vector<int> AddCustomCommandDialog::GetButtonSequence()
{
	return m_buttonSequence;
}

std::vector<std::string> AddCustomCommandDialog::GetButtonSequenceNames()
{
	return m_buttonSequenceNames;
}

CustomButtonSequence::ActionType AddCustomCommandDialog::GetActionType()
{
	return m_actionType;
}

std::string AddCustomCommandDialog::GetActionTypeName()
{
	return m_actionTypeName;
}

std::vector<std::string> AddCustomCommandDialog::GetActionParameters()
{
	return m_actionParameters;
}


// List model:
ButtonSequenceModel::ButtonSequenceModel(QObject* parent):
	QAbstractListModel(parent)
{
	
}

int ButtonSequenceModel::rowCount(const QModelIndex& parent) const
{
	return m_selectedButtons.size();
}
QVariant ButtonSequenceModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}

	if (index.column() == 0)
	{
		return m_selectedButtons[index.row()];
	}
}

void ButtonSequenceModel::RemoveRow(int row)
{
	emit beginRemoveRows(QModelIndex(), row, row);
	m_selectedButtons.remove(row);
	emit endRemoveRows();
}

void ButtonSequenceModel::InsertRow(const std::string& buttonName)
{
	int lastIndex = rowCount();
	emit beginInsertRows(QModelIndex(), lastIndex, lastIndex);

	m_selectedButtons.push_back(QString::fromStdString(buttonName));

	emit endInsertRows();
}