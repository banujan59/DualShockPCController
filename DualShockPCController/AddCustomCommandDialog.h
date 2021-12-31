#pragma once

#include <vector>
#include <QDialog>
#include "ui_AddCustomCommandDialog.h"
#include "DualShockController.h"

class ButtonSequenceModel : public QAbstractListModel
{
	Q_OBJECT
public:
	ButtonSequenceModel(QObject* parent = nullptr);
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	void RemoveRow(int row);
	void InsertRow(const std::string& buttonName);

private:
	QList<QString> m_selectedButtons;
};


class AddCustomCommandDialog : public QDialog
{
	Q_OBJECT

public:
	AddCustomCommandDialog(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);
	~AddCustomCommandDialog();

	std::string GetCommandName();
	std::vector<int> GetButtonSequence();
	std::vector<std::string> GetButtonSequenceNames();
	CustomButtonSequence::ActionType GetActionType();
	std::string GetActionTypeName();
	std::vector<std::string> GetActionParameters();

private slots:
	void HandleActionTypeChanged(int index);
	void HandleAddNewButtonSequence();
	void HandleDeleteButtonSequence();

	void HandleOkButtonClicked();

private:
	Ui::AddCustomCommandDialog ui;

	DualShockController* m_pDualShockController;
	std::map<CustomButtonSequence::ActionType, std::string> m_actionTypeNames;
	std::map<int, std::string> m_dsButtonNames;

	ButtonSequenceModel* m_buttonSequenceListModel;

	std::string m_commandName;
	std::vector<int> m_buttonSequence;
	std::vector<std::string> m_buttonSequenceNames;
	CustomButtonSequence::ActionType m_actionType;
	std::string m_actionTypeName;
	std::vector<std::string> m_actionParameters;
};
