#pragma once

#include <QtWidgets/QMainWindow>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QTreeWidget>
#include <QVector>

#include <QFile>
#include <QApplication>

#include "DialogItem.h"
#include "ui_MainWindow.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void InitSignalSlots(); //��ʼ���źŲ�

	void InitModel(); //��ʼ��Model
	void InitTable(); //��ʼ��Table

	void CreateData(ItemData* pItemData); //�����ݲ���mIteam
	void ClearData(); //���ȫ������

	void OpenDB(QString FileName); //�����ݿ��ļ���ȡ
	void SaveToDB(QString FileName); //�������ݵ��ļ�
	void InsertToDB(ItemData* pItemData); //�������ݵ����ݿ�
	void UpdateToDB(ItemData* pItemData); //�������ݿ��ڼ�¼
	void DeleteFromDB(int JobNumber); //�����ݿ�ɾ����¼

private slots:
	void on_actionOpen_clicked(bool checked);
	void on_actionSave_clicked(bool checked);
	void on_actionAbout_clicked();

	void on_ButtonSearch_clicked(); //�ؼ��ֹ�������
	void on_ButtonAdd_clicked(); //���
	void on_ButtonDelete_clicked(); //ɾ��

	void slotAddItem(ItemData* pItemData); //������ݵ���׼����ģ��
	void on_comboBoxFilter_currentTextChanged(const QString& TempText); //ְ�ƹ���������
	void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous); //ѧԺרҵĿ¼����
	void on_tableView_doubleClicked(const QModelIndex& index); //˫���޸ĵ�ǰ������

private:
	ItemData* SearchItem(int TempJobNumber); //��ItemData���Ҽ�¼
	void DeleteItem(int TempJobNumber); //��Tableɾ����¼

private:
	Ui::MainWindow* ui;
	QString mDefaultDBPath; //���ݿ�·��
	QStandardItemModel* mModel; 

	QVector<ItemData*> mItems; //��ʦ����

	QString mFilterDepartment; //ѧԺ������
	QString mFilterMajor; //רҵ������
	QString mFilterTitle; //ְ�ƹ�����
	QString mFilterKey; //�ؼ��ʹ�����
};