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
	ui->lineEditJobNumber->setValidator(Validator); //������ʽ�޶�����ֻ����������

	if (mItem)
	{
		ui->btnAdd->setText("�޸�");

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
		ui->labelTip->setText("��ʦ���Ų���Ϊ��");

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
		//�޸�����
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
		//��������
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

		ui->labelTip->setText("����¼��ɹ�!");
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
	case 0:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��"); break;
	case 1:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "Ӧ������ѧ" << "��Ϣ������ѧ" << "������Ϣ��ѧ�뼼��" << "��Ϣ������ѧ" << "��ѧ��Ӧ����ѧ"); break;
	case 2:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "���ĵ��������滮" << "����ѧ" << "����滮" << "�羰԰��" << "������Ƽ���" << "���й滮" << "��Դ���������滮����" << "���۽������"); break;
	case 3:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "��ľ����" << "���е��¿ռ乤��" << "���Ͽ�ѧ�빤��"); break;
	case 4:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "����ˮ��ѧ�빤��" << "��������" << "������ѧ" << "��Դ�붯������" << "������������ԴӦ�ù���" << "�����������豸����" << "��ˮ��ˮ����" << "�����붯������"); break;
	case 5:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "��������" << "��е������켰���Զ���" << "��ͨ����" << "���ϳ��ͼ����ƹ���" << "�������񹤳�" << "��ͨ����" << "��ȫ����" << "��ؼ���������"); break;
	case 6:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "�������������ܻ�" << "ͨ�Ź���" << "�������" << "�������ѧ�뼼��" << "������Ϣ����" << "�Զ���" << "���繤��" << "�����Զ�������" << "�������̼����Զ���" << "�������������" << "�����Ӧ�ü���"); break;
	case 7:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "����" << "Ӣ��" << "��������ѧ" << "��Ṥ��" << "���ѧ" << "Ӱ�Ӷ���" << "������" << "����"); break;
	case 8:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "��װ��������" << "���ֱ���" << "��װ����빤��" << "�滭" << "�Ӿ��������" << "�������" << "��Ʒ���" << "��ҵ���" << "�������"); break;
	case 9:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "���̹���" << "���ز����������" << "��������" << "��Ϣ��������Ϣϵͳ" << "�������" << "������Դ����" << "��ҵ����"); break;
	case 10:ui->comboBoxMajor->addItems(QStringList() << "��ѡ��" << "���ʾ�����ó��" << "�������" << "����ѧ" << "ͳ��ѧ" << "��������" << "�г�Ӫ��" << "���ѧ" << "��������"); break;
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
	mAllGenders << "��ѡ��" << "��" << "Ů";
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
	mAllEducations << "��ѡ��" << "��ʿ������" << "˶ʿ" << "��ѧ����" << "��ר��ר��ѧУ" << "����" << "���м�����";
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
	mAllDepartments << "��ѡ��" << "��ѧԺ" << "���������滮ѧԺ" << "��ľ����ѧԺ" << "��������������ѧԺ" << "��е����������ѧԺ" << "��Ϣ����ƹ���ѧԺ" << "�����������ѧԺ" << "���������ѧԺ" << "������ѧԺ" << "��ѧԺ";
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
	mAllMajors << "��ѡ��" << "Ӧ������ѧ" << "��Ϣ������ѧ" << "������Ϣ��ѧ�뼼��" << "��Ϣ������ѧ" << "��ѧ��Ӧ����ѧ"
		<< "���ĵ��������滮" << "����ѧ" << "����滮" << "�羰԰��" << "������Ƽ���" << "���й滮" << "��Դ���������滮����" << "���۽������"
		<< "��ľ����" << "���е��¿ռ乤��" << "���Ͽ�ѧ�빤��"
		<< "����ˮ��ѧ�빤��" << "��������" << "������ѧ" << "��Դ�붯������" << "������������ԴӦ�ù���" << "�����������豸����" << "��ˮ��ˮ����" << "�����붯������"
		<< "��������" << "��е������켰���Զ���" << "��ͨ����" << "���ϳ��ͼ����ƹ���" << "�������񹤳�" << "��ͨ����" << "��ȫ����" << "��ؼ���������"
		<< "�������������ܻ�" << "ͨ�Ź���" << "�������" << "�������ѧ�뼼��" << "������Ϣ����" << "�Զ���" << "���繤��" << "�����Զ�������" << "�������̼����Զ���" << "�������������" << "�����Ӧ�ü���"
		<< "����" << "Ӣ��" << "��������ѧ" << "��Ṥ��" << "���ѧ" << "Ӱ�Ӷ���" << "������" << "����"
		<< "��װ��������" << "���ֱ���" << "��װ����빤��" << "�滭" << "�Ӿ��������" << "�������" << "��Ʒ���" << "��ҵ���" << "�������"
		<< "���̹���" << "���ز����������" << "��������" << "��Ϣ��������Ϣϵͳ" << "�������" << "������Դ����" << "��ҵ����"
		<< "���ʾ�����ó��" << "�������" << "����ѧ" << "ͳ��ѧ" << "��������" << "�г�Ӫ��" << "���ѧ" << "��������";
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
	mAllMajors << "��ѡ��" << "Ӧ������ѧ" << "��Ϣ������ѧ" << "������Ϣ��ѧ�뼼��" << "��Ϣ������ѧ" << "��ѧ��Ӧ����ѧ"
		<< "���ĵ��������滮" << "����ѧ" << "����滮" << "�羰԰��" << "������Ƽ���" << "���й滮" << "��Դ���������滮����" << "���۽������"
		<< "��ľ����" << "���е��¿ռ乤��" << "���Ͽ�ѧ�빤��"
		<< "����ˮ��ѧ�빤��" << "��������" << "������ѧ" << "��Դ�붯������" << "������������ԴӦ�ù���" << "�����������豸����" << "��ˮ��ˮ����" << "�����붯������"
		<< "��������" << "��е������켰���Զ���" << "��ͨ����" << "���ϳ��ͼ����ƹ���" << "�������񹤳�" << "��ͨ����" << "��ȫ����" << "��ؼ���������"
		<< "�������������ܻ�" << "ͨ�Ź���" << "�������" << "�������ѧ�뼼��" << "������Ϣ����" << "�Զ���" << "���繤��" << "�����Զ�������" << "�������̼����Զ���" << "�������������" << "�����Ӧ�ü���"
		<< "����" << "Ӣ��" << "��������ѧ" << "��Ṥ��" << "���ѧ" << "Ӱ�Ӷ���" << "������" << "����"
		<< "��װ��������" << "���ֱ���" << "��װ����빤��" << "�滭" << "�Ӿ��������" << "�������" << "��Ʒ���" << "��ҵ���" << "�������"
		<< "���̹���" << "���ز����������" << "��������" << "��Ϣ��������Ϣϵͳ" << "�������" << "������Դ����" << "��ҵ����"
		<< "���ʾ�����ó��" << "�������" << "����ѧ" << "ͳ��ѧ" << "��������" << "�г�Ӫ��" << "���ѧ" << "��������";
	return mAllMajors.indexOf(StrMajor);
}

QString DialogItem::TitleToString(int Title)
{
	QStringList mAllTitles;
	mAllTitles << "��ѡ��" << "����" << "������" << "��ʦ" << "����";
	if (Title >= 0 && Title < mAllTitles.size())
	{
		return mAllTitles[Title];
	}
	else
	{
		return QString("Undefined");
	}
}