#include "AddCustomCommandDialog.h"
#include <QFileDialog>
#include <QMessageBox>

AddCustomCommandDialog::AddCustomCommandDialog(DualShockController* pDualShockController, QWidget* parent)
	: QDialog(parent),
	m_pDualShockController(pDualShockController)
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

	connect(ui.addButtonToSequenceButton, &QPushButton::clicked, this, &AddCustomCommandDialog::HandleAddNewButtonSequence);
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

		m_buttonSequence.push_back(it->first);
		m_buttonSequenceNames.push_back(it->second);
		m_buttonSequenceListModel->InsertRow(it->second);
	}

	else if(index > 0) // because the field is editable, invalid data can be entered
	{
		ui.buttonSelector->removeItem(index);
	}

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