#include "DialogItem.h"

#include <QMessageBox>

ItemData::ItemData()
{
	nTeacherJobNumber = 197010001;

	szTeacherName = "";
	nTeacherGender = 0;
	nTeacherAge = 0;
	szTeacherID = "0";

	nTeahcerEducation = 0;
	fTeacherSalary = 0.0;
	szTeacherMobile = "";

	nTeacherDepartment = 0;
	nTeacherMajor = 0;
	nTeacherTitle = 0;

	szMemo = "";
}

DialogItem::DialogItem(ItemData* pItemData, QWidget* parent) :
	QDialog(parent),
	ui(new Ui::DialogItem)
{
	ui->setupUi(this);

	mItem = pItemData;

	QFont font("Microsoft YaHei", 10, 75);

	ui->labelTip->setFont(font);
	ui->labelTip->setStyleSheet("QLabel{ color:red }");
	ui->lineEditJobNumber->setFocus();

	QRegExp RegExp("[1-9][0-9]+$");
	QValidator* Validator = new QRegExpValidator(RegExp, ui->lineEditJobNumber);
	ui->lineEditJobNumber->setValidator(Validator); //正则表达式限定工号只能输入数字

	if (mItem)
	{
		ui->btnAdd->setText("修改");

		ui->lineEditJobNumber->setText(QString::number(mItem->nTeacherJobNumber));
		ui->lineEditJobNumber->setEnabled(false);

		ui->lineEditName->setText(mItem->szTeacherName);
		ui->comboBoxGender->setCurrentIndex(mItem->nTeacherGender);
		ui->lineEditAge->setText(QString::number(mItem->nTeacherAge));
		ui->lineEditID->setText(mItem->szTeacherID);

		ui->comboBoxEducation->setCurrentIndex(mItem->nTeahcerEducation);
		ui->lineEditSalary->setText(QString::number(mItem->fTeacherSalary));
		ui->lineEditMobile->setText(mItem->szTeacherMobile);

		ui->comboBoxDepartment->setCurrentIndex(mItem->nTeacherDepartment);
		ui->comboBoxMajor->setCurrentText(MajorToString(mItem->nTeacherMajor));
		ui->comboBoxTitle->setCurrentIndex(mItem->nTeacherTitle);

		ui->textEditMemo->setText(mItem->szMemo);
	}
}

DialogItem::~DialogItem()
{
	delete ui;
}

void DialogItem::on_btnAdd_clicked()
{
	if (ui->lineEditJobNumber->text().isEmpty())
	{
		ui->labelTip->setText("教师工号不能为空");

		return;
	}
	ItemData* pItemData = new ItemData();

	pItemData->nTeacherJobNumber = ui->lineEditJobNumber->text().toInt();

	pItemData->szTeacherName = ui->lineEditName->text();
	pItemData->nTeacherGender = ui->comboBoxGender->currentIndex();
	pItemData->nTeacherAge = ui->lineEditAge->text().toInt();
	pItemData->szTeacherID = ui->lineEditID->text();

	pItemData->nTeahcerEducation = ui->comboBoxEducation->currentIndex();
	pItemData->fTeacherSalary = ui->lineEditSalary->text().toFloat();
	pItemData->szTeacherMobile = ui->lineEditMobile->text();

	pItemData->nTeacherDepartment = ui->comboBoxDepartment->currentIndex();
	pItemData->nTeacherMajor = StringToMajor(ui->comboBoxMajor->currentText());
	pItemData->nTeacherTitle = ui->comboBoxTitle->currentIndex();

	pItemData->szMemo = ui->textEditMemo->toPlainText();

	if (mItem)
	{
		//修改数据
		mItem->nTeacherJobNumber = pItemData->nTeacherJobNumber;

		mItem->szTeacherName = pItemData->szTeacherName;
		mItem->nTeacherGender = pItemData->nTeacherGender;
		mItem->nTeacherAge = pItemData->nTeacherAge;
		mItem->szTeacherID = pItemData->szTeacherID;

		mItem->nTeahcerEducation = pItemData->nTeahcerEducation;
		mItem->fTeacherSalary = pItemData->fTeacherSalary;
		mItem->szTeacherMobile = pItemData->szTeacherMobile;

		mItem->nTeacherDepartment = pItemData->nTeacherDepartment;
		mItem->nTeacherMajor = pItemData->nTeacherMajor;
		mItem->nTeacherTitle = pItemData->nTeacherTitle;

		mItem->szMemo = pItemData->szMemo;

		accept();
	}
	else
	{
		//新增数据
		emit sigAddItem(pItemData);

		ui->lineEditJobNumber->setText("");

		ui->lineEditName->setText("");
		ui->comboBoxGender->setCurrentIndex(0);
		ui->lineEditAge->setText("");
		ui->lineEditID->setText("");

		ui->comboBoxEducation->setCurrentIndex(0);
		ui->lineEditSalary->setText("");
		ui->lineEditMobile->setText("");

		ui->comboBoxDepartment->setCurrentIndex(0);
		ui->comboBoxMajor->setCurrentIndex(0);
		ui->comboBoxTitle->setCurrentIndex(0);

		ui->textEditMemo->setText("");

		ui->lineEditJobNumber->setFocus();

		ui->labelTip->setText("数据录入成功!");
	}
}

void DialogItem::on_btnCancel_clicked()
{
	reject();
}

void DialogItem::on_comboBoxDepartment_currentIndexChanged(int Department)
{
	ui->comboBoxMajor->clear();

	switch (Department)
	{
	case 0:ui->comboBoxMajor->addItems(QStringList() << "请选择"); break;
	case 1:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "应用物理学" << "信息与计算科学" << "电子信息科学与技术" << "信息与计算科学" << "数学与应用数学"); break;
	case 2:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "人文地理与城乡规划" << "建筑学" << "城乡规划" << "风景园林" << "建筑设计技术" << "城市规划" << "资源环境与城乡规划管理" << "景观建筑设计"); break;
	case 3:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "土木工程" << "城市地下空间工程" << "材料科学与工程"); break;
	case 4:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "给排水科学与工程" << "环境工程" << "环境科学" << "能源与动力工程" << "建筑环境与能源应用工程" << "建筑环境与设备工程" << "给水排水工程" << "热能与动力工程"); break;
	case 5:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "车辆工程" << "机械设计制造及其自动化" << "交通运输" << "材料成型及控制工程" << "汽车服务工程" << "交通工程" << "安全工程" << "测控技术与仪器"); break;
	case 6:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "建筑电气与智能化" << "通信工程" << "软件工程" << "计算机科学与技术" << "电子信息工程" << "自动化" << "网络工程" << "电气自动化技术" << "电气工程及其自动化" << "电力工程与管理" << "计算机应用技术"); break;
	case 7:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "日语" << "英语" << "汉语言文学" << "社会工作" << "广告学" << "影视动画" << "朝鲜语" << "德语"); break;
	case 8:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "服装与服饰设计" << "音乐表演" << "服装设计与工程" << "绘画" << "视觉传达设计" << "环境设计" << "产品设计" << "工业设计" << "艺术设计"); break;
	case 9:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "工程管理" << "房地产开发与管理" << "物流管理" << "信息管理与信息系统" << "工程造价" << "土地资源管理" << "工业工程"); break;
	case 10:ui->comboBoxMajor->addItems(QStringList() << "请选择" << "国际经济与贸易" << "财务管理" << "经济学" << "统计学" << "电子商务" << "市场营销" << "会计学" << "国际商务"); break;
	default:break;
	}
}

void DialogItem::on_lineEditJobNumber_TextChanged(const QString& JobNumber)
{
	ui->labelTip->setText("");
}

QString DialogItem::GenderToString(int Gender)
{
	QStringList mAllGenders;
	mAllGenders << "请选择" << "男" << "女";
	if (Gender >= 0 && Gender < mAllGenders.size())
	{
		return mAllGenders[Gender];
	}
	else
	{
		return QString("Undefined");
	}
}

QString DialogItem::EducationToString(int Education)
{
	QStringList mAllEducations;
	mAllEducations << "请选择" << "博士及以上" << "硕士" << "大学本科" << "大专和专科学校" << "高中" << "初中及以下";
	if (Education >= 0 && Education < mAllEducations.size())
	{
		return mAllEducations[Education];
	}
	else
	{
		return QString("Undefined");
	}
}

QString DialogItem::DepartmentToString(int Department)
{
	QStringList mAllDepartments;
	mAllDepartments << "请选择" << "理学院" << "建筑与城乡规划学院" << "土木工程学院" << "环境与市政工程学院" << "机械与汽车工程学院" << "信息与控制工程学院" << "人文与外国语学院" << "艺术与设计学院" << "管理工程学院" << "商学院";
	if (Department >= 0 && Department < mAllDepartments.size())
	{
		return mAllDepartments[Department];
	}
	else
	{
		return QString("Undefined");
	}
}

QString DialogItem::MajorToString(int Major)
{
	QStringList mAllMajors;
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
	if (Major >= 0 && Major < mAllMajors.size())
	{
		return mAllMajors[Major];
	}
	else
	{
		return QString("Undefined");
	}
}

int DialogItem::StringToMajor(QString StrMajor)
{
	QStringList mAllMajors;
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
	return mAllMajors.indexOf(StrMajor);
}

QString DialogItem::TitleToString(int Title)
{
	QStringList mAllTitles;
	mAllTitles << "请选择" << "教授" << "副教授" << "讲师" << "助教";
	if (Title >= 0 && Title < mAllTitles.size())
	{
		return mAllTitles[Title];
	}
	else
	{
		return QString("Undefined");
	}
}