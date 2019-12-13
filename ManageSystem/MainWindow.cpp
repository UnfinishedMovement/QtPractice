#include <QListView>
#include <QFileDialog>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include <QSqlQuery>
#include <QSqlDataBase>
#include <QSqlError>

#include <QDebug>

#include "MainWindow.h"

MainWindow::MainWindow(QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	mDefaultDBPath = QCoreApplication::applicationDirPath() + "\\SQLiteData.db";

	mFilterDepartment = "";
	mFilterMajor = "";
	mFilterTitle = "";
	mFilterKey = "";

	this->setWindowFlags(Qt::FramelessWindowHint);

	//ui->centralWidget->setMouseTracking(true);

	InitSignalSlots();

	OpenDB(mDefaultDBPath);
	InitTable();

	ui->treeWidget->expandAll();
	ui->treeWidget->setCurrentItem(ui->treeWidget->itemAt(0, 0));

	ui->comboBoxFilter->setView(new QListView());
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::InitSignalSlots()
{
	connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(on_actionOpen_clicked(bool)));
	connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(on_actionSave_clicked(bool)));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(on_actionAbout_clicked()));
}

void MainWindow::InitModel()
{
	mModel->removeRows(0, mModel->rowCount());
	for (int i = 0; i < mItems.size(); i++)
	{
		ItemData* pItemData = mItems[i];
		QString TempDepartment = DialogItem::DepartmentToString(pItemData->nTeacherDepartment);
		QString TempMajor = DialogItem::MajorToString(pItemData->nTeacherMajor);
		QString TempTitle = DialogItem::TitleToString(pItemData->nTeacherTitle);
		//学院过滤
		if (!mFilterDepartment.isEmpty())
		{
			if (mFilterDepartment != TempDepartment)
			{
				continue;
			}
		}
		//专业过滤
		if (!mFilterMajor.isEmpty())
		{
			if (mFilterMajor != TempMajor)
			{
				continue;
			}
		}
		//职称过滤
		if (!mFilterTitle.isEmpty())
		{
			if (mFilterTitle != TempTitle)
			{
				continue;
			}
		}
		//关键字过滤
		if (!mFilterKey.isEmpty())
		{
			QStringList TempStrList;

			TempStrList << QString::number(pItemData->nTeacherJobNumber) << pItemData->szTeacherName << pItemData->szTeacherID << pItemData->szTeacherMobile;
			bool JudgeFind = false;
			foreach(QString TempS, TempStrList)
			{
				if (TempS.indexOf(mFilterKey) != -1)
				{
					JudgeFind = true;
					break;
				}
			}
			if (!JudgeFind)
			{
				continue;
			}
		}

		QList<QStandardItem*> TempItems;

		QStandardItem* TempItem1 = new QStandardItem(QString::number(pItemData->nTeacherJobNumber));

		QStandardItem* TempItem2 = new QStandardItem(pItemData->szTeacherName);
		QStandardItem* TempItem3 = new QStandardItem(DialogItem::GenderToString(pItemData->nTeacherGender));
		QStandardItem* TempItem4 = new QStandardItem(QString::number(pItemData->nTeacherAge));
		QStandardItem* TempItem5 = new QStandardItem(pItemData->szTeacherID);

		QStandardItem* TempItem6 = new QStandardItem(DialogItem::EducationToString(pItemData->nTeahcerEducation));
		QStandardItem* TempItem7 = new QStandardItem(QString::number(pItemData->fTeacherSalary));
		QStandardItem* TempItem8 = new QStandardItem(pItemData->szTeacherMobile);

		QStandardItem* TempItem9 = new QStandardItem(DialogItem::DepartmentToString(pItemData->nTeacherDepartment));
		QStandardItem* TempItem10 = new QStandardItem(DialogItem::MajorToString(pItemData->nTeacherMajor));
		QStandardItem* TempItem11 = new QStandardItem(DialogItem::TitleToString(pItemData->nTeacherTitle));

		QStandardItem* TempItem12 = new QStandardItem(pItemData->szMemo);

		TempItems.append(TempItem1);

		TempItems.append(TempItem2);
		TempItems.append(TempItem3);
		TempItems.append(TempItem4);
		TempItems.append(TempItem5);

		TempItems.append(TempItem6);
		TempItems.append(TempItem7);
		TempItems.append(TempItem8);

		TempItems.append(TempItem9);
		TempItems.append(TempItem10);
		TempItems.append(TempItem11);

		TempItems.append(TempItem12);
		mModel->appendRow(TempItems);
	}
}

void MainWindow::InitTable()
{
	QTableView* pTable = ui->tableView;
	pTable->setEditTriggers(QTreeView::NoEditTriggers); //不能编辑
	pTable->setSelectionBehavior(QTreeView::SelectRows); //一次选中整行
	//pTable->setAlternatingRowColors(true); //隔行自动变色

	QString TreeStyle = "QTableView::item{ height: 25px }";
	pTable->setStyleSheet(TreeStyle);

	QStringList AllHeaders;
	AllHeaders << QString("教师工号")
		<< QString("教师姓名") << QString("教师性别") << QString("教师年龄") << QString("教师身份证号")
		<< QString("教师受教育程度") << QString("教师工资") << QString("教师手机号")
		<< QString("教师学院") << QString("教师专业") << QString("教师职称")
		<< QString("备注");
	mModel = new QStandardItemModel(pTable);
	mModel->setHorizontalHeaderLabels(AllHeaders);

	InitModel();
	pTable->setModel(mModel);
	for (int i = 0; i < AllHeaders.size(); i++)
	{
		switch (i)
		{
		case 0:pTable->horizontalHeader()->resizeSection(i, 100);break;
		case 1:pTable->horizontalHeader()->resizeSection(i, 80);break;
		case 2:pTable->horizontalHeader()->resizeSection(i, 80);break;
		case 3:pTable->horizontalHeader()->resizeSection(i, 80);break;
		case 4:pTable->horizontalHeader()->resizeSection(i, 190);break;
		case 5:pTable->horizontalHeader()->resizeSection(i, 130);break;
		case 6:pTable->horizontalHeader()->resizeSection(i, 100);break;
		case 7:pTable->horizontalHeader()->resizeSection(i, 140);break;
		case 8:pTable->horizontalHeader()->resizeSection(i, 150);break;
		case 9:pTable->horizontalHeader()->resizeSection(i, 180);break;
		case 10:pTable->horizontalHeader()->resizeSection(i, 100);break;
		case 11:pTable->horizontalHeader()->resizeSection(i, 300);break;
		default:break;
		}
	}

	QModelIndex RootIndex = pTable->rootIndex();
	QModelIndex SetIndex = mModel->index(0, 0, RootIndex);
	pTable->setCurrentIndex(SetIndex);
}

void MainWindow::CreateData(ItemData* pItemData)
{
	mItems.push_back(pItemData);
}

void MainWindow::ClearData()
{
	for (int i = 0; i < mItems.size(); i++)
	{
		ItemData* pItemData = mItems[i];
		if (pItemData)
		{
			delete pItemData;
			pItemData = NULL;
		}
	}
	mItems.clear();
}

void MainWindow::OpenDB(QString FileName)
{
	QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
	DB.setDatabaseName(FileName);
	if (!DB.open())
	{
		QMessageBox::warning(this, "错误", "打开数据库文件失败", QMessageBox::Ok);
	}
	else
	{
		qDebug() << "Database Opened";

		mDefaultDBPath = FileName;

		QSqlQuery Sql_query;
		QString Sql_select_all = "select * from Teacher";

		Sql_query.prepare(Sql_select_all);
		Sql_query.exec();
		if (Sql_query.isActive())
		{
			while (Sql_query.next())
			{
				ItemData* pItemData = new ItemData();

				pItemData->nTeacherJobNumber = Sql_query.value(0).toInt();

				pItemData->szTeacherName = Sql_query.value(1).toString();
				pItemData->nTeacherGender = Sql_query.value(2).toInt();
				pItemData->nTeacherAge = Sql_query.value(3).toInt();
				pItemData->szTeacherID = Sql_query.value(4).toString();

				pItemData->nTeahcerEducation = Sql_query.value(5).toInt();
				pItemData->fTeacherSalary = Sql_query.value(6).toFloat();
				pItemData->szTeacherMobile = Sql_query.value(7).toString();

				pItemData->nTeacherDepartment = Sql_query.value(8).toInt();
				pItemData->nTeacherMajor = Sql_query.value(9).toInt();
				pItemData->nTeacherTitle = Sql_query.value(10).toInt();

				pItemData->szMemo = Sql_query.value(11).toString();

				mItems.push_back(pItemData);
			}
		}
		else
		{
			qDebug() << Sql_query.lastError();
		}
	}
	DB.close();
}

void MainWindow::SaveToDB(QString FileName)
{
	QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
	DB.setDatabaseName(FileName);
	if (DB.open())
	{
		qDebug() << "Database Opened";

		QSqlQuery Sql_query;
		QString Sql_create = "create table if not exists Teacher (JobNumber int primary key not null,"
			"Name varchar(20),"
			"Gender int,"
			"Age int,"
			"ID varchar(18),"
			"Education int,"
			"Salary float,"
			"Mobile varchar(11),"
			"Department int,"
			"Major int,"
			"Title int,"
			"Memo varchar(100))";//创建数据表

		Sql_query.prepare(Sql_create);

		if (!Sql_query.exec())
		{
			qDebug() << QObject::tr("Table create failed");
			qDebug() << Sql_query.lastError();
		}
		else
		{
			qDebug() << "Table create successed";

			for (int i = 0; i < mItems.size(); i++)
			{
				ItemData* pItemData = mItems[i];

				Sql_query.prepare("insert into Teacher values(:JobNumber,:Name,:Gender,:Age,:ID,:Education,:Salary,:Mobile,:Department,:Major,:Title,:Memo)");

				Sql_query.bindValue(":JobNumber", pItemData->nTeacherJobNumber);
				Sql_query.bindValue(":Name", pItemData->szTeacherName);
				Sql_query.bindValue(":Gender", pItemData->nTeacherGender);
				Sql_query.bindValue(":Age", pItemData->nTeacherAge);
				Sql_query.bindValue(":ID", pItemData->szTeacherID);
				Sql_query.bindValue(":Education", pItemData->nTeahcerEducation);
				Sql_query.bindValue(":Salary", pItemData->fTeacherSalary);
				Sql_query.bindValue(":Mobile", pItemData->szTeacherMobile);
				Sql_query.bindValue(":Department", pItemData->nTeacherDepartment);
				Sql_query.bindValue(":Major", pItemData->nTeacherMajor);
				Sql_query.bindValue(":Title", pItemData->nTeacherTitle);
				Sql_query.bindValue(":Memo", pItemData->szMemo);

				if (!Sql_query.exec())
				{
					qDebug() << Sql_query.lastError();
				}
				else
				{
					qDebug() << "录入记录成功";
				}
			}
		}
	}
	DB.close();
}

void MainWindow::InsertToDB(ItemData* pItemData)
{
	QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
	DB.setDatabaseName(mDefaultDBPath);
	if (DB.open())
	{
		qDebug() << "Database Opened";

		QSqlQuery Sql_query;
		QString Sql_create = "create table if not exists Teacher (JobNumber int primary key not null,"
			"Name varchar(20),"
			"Gender int,"
			"Age int,"
			"ID varchar(18),"
			"Education int,"
			"Salary float,"
			"Mobile varchar(11),"
			"Department int,"
			"Major int,"
			"Title int,"
			"Memo varchar(100))";//创建数据表

		Sql_query.prepare(Sql_create);
		Sql_query.exec();

		Sql_query.prepare("insert into Teacher (JobNumber,Name,Gender,Age,ID,Education,Salary,Mobile,Department,Major,Title,Memo) "
			"values(:JobNumber,:Name,:Gender,:Age,:ID,:Education,:Salary,:Mobile,:Department,:Major,:Title,:Memo)");
		//Sql_query.prepare("insert into Teacher values(197000001,\"张三\",1,0,\"110101197001010016\",2,0.0,\"13000000001\",2,2,2,\"备注\")");

		Sql_query.bindValue(":JobNumber", pItemData->nTeacherJobNumber);
		Sql_query.bindValue(":Name", pItemData->szTeacherName);
		Sql_query.bindValue(":Gender", pItemData->nTeacherGender);
		Sql_query.bindValue(":Age", pItemData->nTeacherAge);
		Sql_query.bindValue(":ID", pItemData->szTeacherID);
		Sql_query.bindValue(":Education", pItemData->nTeahcerEducation);
		Sql_query.bindValue(":Salary", pItemData->fTeacherSalary);
		Sql_query.bindValue(":Mobile", pItemData->szTeacherMobile);
		Sql_query.bindValue(":Department", pItemData->nTeacherDepartment);
		Sql_query.bindValue(":Major", pItemData->nTeacherMajor);
		Sql_query.bindValue(":Title", pItemData->nTeacherTitle);
		Sql_query.bindValue(":Memo", pItemData->szMemo);

		if (!Sql_query.exec())
		{
			qDebug() << Sql_query.lastError();
		}
		else
		{
			qDebug() << "插入记录成功";
		}
	}
	DB.close();
}

void MainWindow::UpdateToDB(ItemData* pItemData)
{
	QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
	DB.setDatabaseName(mDefaultDBPath);
	if (DB.open())
	{
		qDebug() << "Database Opened";

		QSqlQuery Sql_query;

		Sql_query.prepare("update Teacher set Name=:Name,Gender=:Gender,Age=:Age,ID=:ID,Education=:Education,Salary=:Salary,Mobile=:Mobile,Department=:Department,"
			"Major=:Major,Title=:Title,Memo=:Memo where JobNumber=:JobNumber");

		Sql_query.bindValue(":Name", pItemData->szTeacherName);
		Sql_query.bindValue(":Gender", pItemData->nTeacherGender);
		Sql_query.bindValue(":Age", pItemData->nTeacherAge);
		Sql_query.bindValue(":ID", pItemData->szTeacherID);
		Sql_query.bindValue(":Education", pItemData->nTeahcerEducation);
		Sql_query.bindValue(":Salary", pItemData->fTeacherSalary);
		Sql_query.bindValue(":Mobile", pItemData->szTeacherMobile);
		Sql_query.bindValue(":Department", pItemData->nTeacherDepartment);
		Sql_query.bindValue(":Major", pItemData->nTeacherMajor);
		Sql_query.bindValue(":Title", pItemData->nTeacherTitle);
		Sql_query.bindValue(":Memo", pItemData->szMemo);

		Sql_query.bindValue(":JobNumber", pItemData->nTeacherJobNumber);

		if (!Sql_query.exec())
		{
			qDebug() << Sql_query.lastError();
		}
		else
		{
			qDebug() << "修改记录成功";
		}
	}
	DB.close();
}

void MainWindow::DeleteFromDB(int JobNumber)
{
	QSqlDatabase DB = QSqlDatabase::addDatabase("QSQLITE");
	DB.setDatabaseName(mDefaultDBPath);
	if (DB.open())
	{
		qDebug() << "Database Opened";

		QSqlQuery Sql_query;

		Sql_query.prepare("delete from Teacher where JobNumber=?");

		Sql_query.bindValue(0, JobNumber);

		if (!Sql_query.exec())
		{
			qDebug() << Sql_query.lastError();
		}
		else
		{
			qDebug() << "删除记录成功";
		}
	}
	DB.close();
}

void MainWindow::on_actionOpen_clicked(bool checked)
{
	QString FileName = QFileDialog::getOpenFileName(this, QString("从文件打开"), ".", "Database Files(*.db)");
	if (FileName.isNull())
	{
		return;
	}
	if (FileName.indexOf(".db") != -1)
	{
		ClearData();
		OpenDB(FileName);
		InitTable();
	}
}

void MainWindow::on_actionSave_clicked(bool checked)
{
	QString FileName = QFileDialog::getSaveFileName(this, QString("保存到文件"), ".", "Database Files(*.db)");
	if (FileName.isNull())
	{
		return;
	}
	if (QFile::exists(FileName))
	{
		QFile::remove(FileName);
	}
	SaveToDB(FileName);
}

void MainWindow::on_actionAbout_clicked()
{
	QMessageBox::about(NULL, "关于","https://github.com/UnfinishedMovement");
}

void MainWindow::on_ButtonSearch_clicked()
{
	QString TempStr = ui->lineEditSearch->text();
	mFilterKey = TempStr;
	InitModel();
}

void MainWindow::on_ButtonAdd_clicked()
{
	DialogItem Dialog(NULL);
	connect(&Dialog, &DialogItem::sigAddItem, this, &MainWindow::slotAddItem);
	Dialog.exec();
}

void MainWindow::on_ButtonDelete_clicked()
{
	if (!ui->tableView->currentIndex().isValid())
	{
		return;
	}

	int TempRow = ui->tableView->currentIndex().row();
	int TempJobNumber = mModel->item(TempRow, 0)->text().toInt();

	DeleteItem(TempJobNumber);
	mModel->removeRow(TempRow);
	DeleteFromDB(TempJobNumber);
}

void MainWindow::slotAddItem(ItemData* pItemData)
{
	mItems.push_back(pItemData);

	QList<QStandardItem*> TempItems;

	QStandardItem* TempItem1 = new QStandardItem(QString::number(pItemData->nTeacherJobNumber));

	QStandardItem* TempItem2 = new QStandardItem(pItemData->szTeacherName);
	QStandardItem* TempItem3 = new QStandardItem(DialogItem::GenderToString(pItemData->nTeacherGender));
	QStandardItem* TempItem4 = new QStandardItem(QString::number(pItemData->nTeacherAge));
	QStandardItem* TempItem5 = new QStandardItem(pItemData->szTeacherID);

	QStandardItem* TempItem6 = new QStandardItem(DialogItem::EducationToString(pItemData->nTeahcerEducation));
	QStandardItem* TempItem7 = new QStandardItem(QString::number(pItemData->fTeacherSalary));
	QStandardItem* TempItem8 = new QStandardItem(pItemData->szTeacherMobile);

	QStandardItem* TempItem9 = new QStandardItem(DialogItem::DepartmentToString(pItemData->nTeacherDepartment));
	QStandardItem* TempItem10 = new QStandardItem(DialogItem::MajorToString(pItemData->nTeacherMajor));
	QStandardItem* TempItem11 = new QStandardItem(DialogItem::TitleToString(pItemData->nTeacherTitle));

	QStandardItem* TempItem12 = new QStandardItem(pItemData->szMemo);

	TempItems.append(TempItem1);

	TempItems.append(TempItem2);
	TempItems.append(TempItem3);
	TempItems.append(TempItem4);
	TempItems.append(TempItem5);

	TempItems.append(TempItem6);
	TempItems.append(TempItem7);
	TempItems.append(TempItem8);

	TempItems.append(TempItem9);
	TempItems.append(TempItem10);
	TempItems.append(TempItem11);

	TempItems.append(TempItem12);
	mModel->appendRow(TempItems);

	InsertToDB(pItemData);
}

void MainWindow::on_comboBoxFilter_currentTextChanged(const QString& TempText)
{
	if (TempText == "所有类型")
	{
		mFilterTitle = "";
	}
	else
	{
		mFilterTitle = TempText;
	}
	InitModel();
}

void MainWindow::on_treeWidget_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	QStringList mAllDepartments, mAllMajors;
	mAllDepartments << "请选择" << "理学院" << "建筑与城乡规划学院" << "土木工程学院" << "环境与市政工程学院" << "机械与汽车工程学院" << "信息与控制工程学院" << "人文与外国语学院" << "艺术与设计学院" << "管理工程学院" << "商学院";

	mAllMajors << "请选择" << "应用物理学" << "信息与计算科学" << "电子信息科学与技术" << "信息与计算科学" << "数学与应用数学"
		<< "人文地理与城乡规划" << "建筑学" << "城乡规划" << "风景园林" << "建筑设计技术" << "城市规划" << "资源环境与城乡规划管理" << "景观建筑设计"
		<< "土木工程" << "城市地下空间工程" << "材料科学与工程"
		<< "给排水科学与工程" << "环境工程" << "环境科学" << "能源与动力工程" << "建筑环境与能源应用工程" << "建筑环境与设备工程" << "给水排水工程" << "热能与动力工程"
		<< "车辆工程" << "机械设计制造及其自动化" << "交通运输" << "材料成型及控制工程" << "汽车服务工程" << "交通工程" << "安全工程" << "测控技术与仪器"
		<< "建筑电气与智能化" << "通信工程" << "软件工程" << "计算机科学与技术" << "电子信息工程" << "自动化" << "网络工程" << "电气自动化技术" << "电气工程及其自动化" << "电力工程与管理" << "计算机应用技术"
		<< "日语" << "英语" << "汉语言文学" << "社会工作" << "广告学" << "影视动画" << "朝鲜语" << "德语"
		<< "服装与服饰设计" << "音乐表演" << "服装设计与工程" << "绘画" << "视觉传达设计" << "环境设计" << "产品设计" << "工业设计" << "艺术设计"
		<< "工程管理" << "房地产开发与管理" << "物流管理" << "信息管理与信息系统" << "工程造价" << "土地资源管理" << "工业工程"
		<< "国际经济与贸易" << "财务管理" << "经济学" << "统计学" << "电子商务" << "市场营销" << "会计学" << "国际商务";

	QString TempStr = current->text(0);
	if (TempStr == "全部/全部")
	{
		mFilterDepartment = "";
		mFilterMajor = "";
	}
	else if (mAllDepartments.contains(TempStr))
	{
		mFilterDepartment = TempStr;
		mFilterMajor = "";
	}
	else if (mAllMajors.contains(TempStr))
	{
		mFilterDepartment = "";
		mFilterMajor = TempStr;
	}
	InitModel();
}

void MainWindow::on_tableView_doubleClicked(const QModelIndex& index)
{
	int TempJobNumber = mModel->item(index.row(), 0)->text().toInt();
	ItemData* pItemData = SearchItem(TempJobNumber);

	if (pItemData)
	{
		DialogItem Dialog(pItemData);
		if (QDialog::Accepted == Dialog.exec())
		{
			mModel->item(index.row(), 0)->setText(QString::number(pItemData->nTeacherJobNumber));

			mModel->item(index.row(), 1)->setText(pItemData->szTeacherName);
			mModel->item(index.row(), 2)->setText(DialogItem::GenderToString(pItemData->nTeacherGender));
			mModel->item(index.row(), 3)->setText(QString::number(pItemData->nTeacherAge));
			mModel->item(index.row(), 4)->setText(pItemData->szTeacherID);

			mModel->item(index.row(), 5)->setText(DialogItem::EducationToString(pItemData->nTeahcerEducation));
			mModel->item(index.row(), 6)->setText(QString::number(pItemData->fTeacherSalary));
			mModel->item(index.row(), 7)->setText(pItemData->szTeacherMobile);

			mModel->item(index.row(), 8)->setText(DialogItem::DepartmentToString(pItemData->nTeacherDepartment));
			mModel->item(index.row(), 9)->setText(DialogItem::MajorToString(pItemData->nTeacherMajor));
			mModel->item(index.row(), 10)->setText(DialogItem::TitleToString(pItemData->nTeacherTitle));

			mModel->item(index.row(), 11)->setText(pItemData->szMemo);

			UpdateToDB(pItemData);
		}
	}
}

ItemData* MainWindow::SearchItem(int TempJobNumber)
{
	for (int i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->nTeacherJobNumber == TempJobNumber)
		{

			return mItems[i];
		}
	}
	return nullptr;
}

void MainWindow::DeleteItem(int TempJobNumber)
{
	for (int i = 0; i < mItems.size(); i++)
	{
		if (mItems[i]->nTeacherJobNumber == TempJobNumber)
		{
			mItems.removeAt(i);
			return;
		}
	}
}