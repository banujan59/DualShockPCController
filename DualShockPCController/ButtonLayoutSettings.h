#pragma once

#include <QWidget>
#include "ui_ButtonLayoutSettings.h"
#include "DualShockController.h"

class ButtonSequenceTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    ButtonSequenceTableModel(QObject* parent = 0);

    void populateData(const QList<QString>& sequenceName, const QList<QString>& sequenceButtons, const QList<QString>& sequenceAction);

    int rowCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int columnCount(const QModelIndex& parent = QModelIndex()) const Q_DECL_OVERRIDE;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    std::string RemoveRow(int row);
    void InsertRow(const std::string& sequenceName, const std::string& sequenceButton, const std::string& sequenceAction);

private:
    QList<QString> tm_sequenceName;
    QList<QString> tm_sequenceButton;
    QList<QString> tm_sequenceAction;
};

class ButtonLayoutSettings : public QWidget
{
	Q_OBJECT

public:
	ButtonLayoutSettings(DualShockController* pDualShockController, QWidget *parent = Q_NULLPTR);
	~ButtonLayoutSettings();

public slots:
	void HandleWidgetEnableStateChange(bool disableState);

private slots:
    void HandleDeleteSequenceButtonClicked() const;
    void HandleAddSequenceButtonClicked() const;

private:
	Ui::ButtonLayoutSettings ui;

	DualShockController* m_pDualShockController;
    ButtonSequenceTableModel* m_buttonSequenceTableModel;

    static void CopyVectorToQList(const std::vector<std::string>& vectorToCopy, QList<QString>& outputList);
};
