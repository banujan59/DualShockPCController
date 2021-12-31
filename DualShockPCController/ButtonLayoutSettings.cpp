#include "ButtonLayoutSettings.h"
#include "AddCustomCommandDialog.h"

#include <QMessageBox>

ButtonLayoutSettings::ButtonLayoutSettings(DualShockController* pDualShockController, QWidget *parent)
	: QWidget(parent),
	m_pDualShockController(pDualShockController)
{
	if (m_pDualShockController == nullptr)
		return;

	ui.setupUi(this);

	auto buttonLayoutNames = m_pDualShockController->GetButtonConfigurationNames();
	std::ranges::for_each(buttonLayoutNames, [&](const std::string& buttonLayoutName)
	{
		ui.buttonLayoutSelector->addItem( QString::fromStdString(buttonLayoutName) );
	});

	connect(ui.deleteSequenceButton, &QPushButton::clicked, this, &ButtonLayoutSettings::HandleDeleteSequenceButtonClicked);
	connect(ui.addSequenceButton, &QPushButton::clicked, this, &ButtonLayoutSettings::HandleAddSequenceButtonClicked);


	// Populate the button sequence table
	std::vector<std::string> commandNames, buttonList, actionType;
	m_pDualShockController->GetAllCustomCommands(commandNames, buttonList, actionType);
	QList<QString> sequenceName, sequenceButtons, sequenceAction;

	CopyVectorToQList(commandNames, sequenceName);
	CopyVectorToQList(buttonList, sequenceButtons);
	CopyVectorToQList(actionType, sequenceAction);

	// Create model:
	m_buttonSequenceTableModel = new ButtonSequenceTableModel(this);

	// Populate model with data:
	m_buttonSequenceTableModel->populateData(sequenceName, sequenceButtons, sequenceAction);

	// Connect model to table view:
	ui.sequenceView->setModel(m_buttonSequenceTableModel);

	// Single row selection only
	ui.sequenceView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.sequenceView->setSelectionMode(QAbstractItemView::SingleSelection);

	// Adjust table header
	ui.sequenceView->verticalHeader()->hide();
	ui.sequenceView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	// Make table header visible and display table:
	ui.sequenceView->horizontalHeader()->setVisible(true);
	ui.sequenceView->show();
}

ButtonLayoutSettings::~ButtonLayoutSettings()
{
}


void ButtonLayoutSettings::HandleWidgetEnableStateChange(bool disableState)
{
	ui.buttonLayoutSelector->setDisabled(disableState);
	ui.addSequenceButton->setDisabled(disableState);
	ui.sequenceView->setDisabled(disableState);
	ui.deleteSequenceButton->setDisabled(disableState);
}

void ButtonLayoutSettings::HandleDeleteSequenceButtonClicked() const
{
	auto indexes = ui.sequenceView->selectionModel()->selectedIndexes();

	int rowToRemove = -1;

	for (const QModelIndex& index : indexes) {
		rowToRemove = index.row();
	}

	if(rowToRemove != -1)
	{
		std::string commandNameToRemove = m_buttonSequenceTableModel->RemoveRow(rowToRemove);
		m_pDualShockController->RemoveCustomCommand(commandNameToRemove);
	}
}

void ButtonLayoutSettings::HandleAddSequenceButtonClicked() const
{
	AddCustomCommandDialog addCustomCommandDialog(m_pDualShockController);
	const int returnVal = addCustomCommandDialog.exec();

	if(returnVal == QDialog::Accepted)
	{
		std::string commandName = addCustomCommandDialog.GetCommandName();
		std::vector<int> buttonSequence = addCustomCommandDialog.GetButtonSequence();
		CustomButtonSequence::ActionType actionType = addCustomCommandDialog.GetActionType();
		std::vector<std::string> actionTypeParameters = addCustomCommandDialog.GetActionParameters();

		// handle duplicate names
		{
			int nameIndex = 1;
			const std::string originalName = commandName;

			while (m_buttonSequenceTableModel->SequenceNameExists(commandName))
			{
				commandName = originalName + "_" + std::to_string(nameIndex);
				nameIndex++;
			}
		}


		if(m_pDualShockController->AddNewCustomCommand(commandName, buttonSequence, actionType, actionTypeParameters))
		{
			std::vector<std::string> buttonSequeneNames = addCustomCommandDialog.GetButtonSequenceNames();
			std::string buttons = "";
			for (auto it = buttonSequeneNames.begin(); it != buttonSequeneNames.end(); it++)
			{
				buttons += *it;

				if (it != buttonSequeneNames.end() - 1)
					buttons += ",";
			}

			m_buttonSequenceTableModel->InsertRow(commandName, buttons, addCustomCommandDialog.GetActionTypeName());
			ui.sequenceView->scrollToBottom();
		}

		else
		{
			QMessageBox msgBox;
			msgBox.setText(QString("Could not add custom command. Causes:\n%1%2").arg("1) Make sure name is unique\n",
				"2) Make sure the sequence is not already used by another custom command."));
			msgBox.setIcon(QMessageBox::Critical);
			msgBox.exec();
		}
	}
}

void ButtonLayoutSettings::CopyVectorToQList(const std::vector<std::string>& vectorToCopy, QList<QString>& outputList)
{
	std::ranges::for_each(vectorToCopy.begin(), vectorToCopy.end(), 
		[&outputList](const std::string& element)
		{
			outputList.append(QString::fromStdString(element));
		}
	);
}



ButtonSequenceTableModel::ButtonSequenceTableModel(QObject* parent) : QAbstractTableModel(parent)
{
}

// Create a method to populate the model with data:
void ButtonSequenceTableModel::populateData(const QList<QString>& sequenceName, const QList<QString>& sequenceButtons, const QList<QString>& sequenceAction)
{
	tm_sequenceName.clear();
	tm_sequenceName = sequenceName;
	tm_sequenceButton.clear();
	tm_sequenceButton = sequenceButtons;
	tm_sequenceAction.clear();
	tm_sequenceAction = sequenceAction;
}

int ButtonSequenceTableModel::rowCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return tm_sequenceName.length();
}

int ButtonSequenceTableModel::columnCount(const QModelIndex& parent) const
{
	Q_UNUSED(parent);
	return 3;
}

QVariant ButtonSequenceTableModel::data(const QModelIndex& index, int role) const
{
	if (!index.isValid() || role != Qt::DisplayRole) {
		return QVariant();
	}

	if (index.column() == 0) 
	{
		return tm_sequenceName[index.row()];
	}

	if (index.column() == 1) 
	{
		return tm_sequenceButton[index.row()];
	}

	if(index.column() == 2)
	{
		return tm_sequenceAction[index.row()];
	}

	return QVariant();
}

QVariant ButtonSequenceTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal) 
	{
		if (section == 0) 
		{
			return QString("Name");
		}

		else if (section == 1) 
		{
			return QString("Sequence");
		}

		else if(section == 2)
		{
			return QString("Action");
		}
	}
	return QVariant();
}

std::string ButtonSequenceTableModel::RemoveRow(int row)
{
	emit beginRemoveRows(QModelIndex(), row, row);

	std::string commandNameToRemove = tm_sequenceName[row].toStdString();

	tm_sequenceName.remove(row);
	tm_sequenceButton.remove(row);
	tm_sequenceAction.remove(row);

	emit endRemoveRows();

	return commandNameToRemove;
}

void ButtonSequenceTableModel::InsertRow(const std::string& sequenceName, const std::string& sequenceButton, const std::string& sequenceAction)
{
	const QString qSequenceName = QString::fromStdString(sequenceName);
	const QString qSequenceButton = QString::fromStdString(sequenceButton);
	const QString qSequenceAction = QString::fromStdString(sequenceAction);

	int lastIndex = rowCount();
	emit beginInsertRows(QModelIndex(), lastIndex, lastIndex);

	tm_sequenceName.push_back(qSequenceName);
	tm_sequenceButton.push_back(qSequenceButton);
	tm_sequenceAction.push_back(qSequenceAction);

	emit endInsertRows();
}

bool ButtonSequenceTableModel::SequenceNameExists(const std::string& nameToCheck)
{
	const QString qNameToCheck = QString::fromStdString(nameToCheck);

	if (std::find(tm_sequenceName.begin(), tm_sequenceName.end(), qNameToCheck) != tm_sequenceName.end())
		return true;

	return false;
}