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

	void InitSignalSlots(); //初始化信号槽

	void InitModel(); //初始化Model
	void InitTable(); //初始化Table

	void CreateData(ItemData* pItemData); //将数据插入mIteam
	void ClearData(); //清除全部数据

	void OpenDB(QString FileName); //从数据库文件读取
	void SaveToDB(QString FileName); //保存数据到文件
	void InsertToDB(ItemData* pItemData); //插入数据到数据库
	void UpdateToDB(ItemData* pItemData); //更新数据库内记录
	void DeleteFromDB(int JobNumber); //从数据库删除记录

private slots:
	void on_actionOpen_clicked(bool checked);
	void on_actionSave_clicked(bool checked);
	void on_actionAbout_clicked();

	void on_ButtonSearch_clicked(); //关键字过滤搜索
	void on_ButtonAdd_clicked(); //添加
	void on_ButtonDelete_clicked(); //删除

	void slotAddItem(ItemData* pItemData); //添加数据到标准数据模型
	void on_comboBoxFilter_currentTextChanged(const QString& TempText); //职称过滤器更改
	void on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous); //学院专业目录更改
	void on_tableView_doubleClicked(const QModelIndex& index); //双击修改当前行数据

private:
	ItemData* SearchItem(int TempJobNumber); //从ItemData查找记录
	void DeleteItem(int TempJobNumber); //从Table删除记录

private:
	Ui::MainWindow* ui;
	QString mDefaultDBPath; //数据库路径
	QStandardItemModel* mModel; 

	QVector<ItemData*> mItems; //教师数据

	QString mFilterDepartment; //学院过滤器
	QString mFilterMajor; //专业过滤器
	QString mFilterTitle; //职称过滤器
	QString mFilterKey; //关键词过滤器
};