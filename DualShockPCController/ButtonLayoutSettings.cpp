#include "ButtonLayoutSettings.h"

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
	QList<QString> sequenceName;
	QList<QString> sequenceButtons;
	QList<QString> sequenceAction;

	// Create some data that is tabular in nature:
	sequenceName.append("Thomas");
	sequenceName.append("Richard");
	sequenceName.append("Harrison");
	sequenceButtons.append("123-456-7890");
	sequenceButtons.append("222-333-4444");
	sequenceButtons.append("333-444-5555");
	sequenceAction.append("Call");
	sequenceAction.append("Call");
	sequenceAction.append("Call");

	// Create model:
	m_buttonSequenceTableModel = new ButtonSequenceTableModel(this);

	// Populate model with data:
	m_buttonSequenceTableModel->populateData(sequenceName, sequenceButtons, sequenceAction);

	// Connect model to table view:
	ui.sequenceView->setModel(m_buttonSequenceTableModel);

	// Single row selection only
	ui.sequenceView->verticalHeader()->hide();
	ui.sequenceView->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.sequenceView->setSelectionMode(QAbstractItemView::SingleSelection);

	// Make table header visible and display table:
	ui.sequenceView->horizontalHeader()->setVisible(true);
	ui.sequenceView->show();
}

ButtonLayoutSettings::~ButtonLayoutSettings()
{
}


void ButtonLayoutSettings::HandleWidgetEnableStateChange(bool disableState)
{
	if(disableState)
	{
		ui.deleteSequenceButton->setDisabled(true);
	}

	else
	{
		ui.deleteSequenceButton->setDisabled(false); // TODO check if an element is selected in the column view before
	}

	ui.buttonLayoutSelector->setDisabled(disableState);
	ui.addSequenceButton->setDisabled(disableState);
	ui.sequenceView->setDisabled(disableState);
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
		m_buttonSequenceTableModel->removeRow(rowToRemove);
	}
}

void ButtonLayoutSettings::HandleAddSequenceButtonClicked() const
{
	std::string name = "Example";
	std::string buttons = "X, O";
	std::string action = "to test";

	m_buttonSequenceTableModel->InsertRow(name, buttons, action);
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

void ButtonSequenceTableModel::removeRow(int row)
{
	emit beginRemoveRows(QModelIndex(), row, row);

	tm_sequenceName.remove(row);
	tm_sequenceButton.remove(row);
	tm_sequenceAction.remove(row);

	emit endRemoveRows();
}

void ButtonSequenceTableModel::InsertRow(const std::string& sequenceName, const std::string& sequenceButton, const std::string& sequenceAction)
{
	const QString qSequenceName = QString::fromStdString(sequenceName);
	const QString qSequenceButton = QString::fromStdString(sequenceButton);
	const QString qSequenceAction = QString::fromStdString(sequenceAction);

	int lastIndex = rowCount() - 1;
	beginInsertRows(QModelIndex(), lastIndex, lastIndex);

	tm_sequenceName.push_back(qSequenceName);
	tm_sequenceButton.push_back(qSequenceButton);
	tm_sequenceAction.push_back(qSequenceAction);

	endInsertRows();
}