#pragma once
#pragma execution_character_set("utf-8")

#include <QDialog>

#include "ui_DialogItem.h"

namespace Ui {
	class DialogItem;
}

class ItemData
{
public:
	ItemData();

	int nTeacherJobNumber; //��ʦ��ţ��������ǿ�

	QString szTeacherName; //��ʦ����
	int nTeacherGender; //��ʦ�Ա�
	int nTeacherAge; //��ʦ����
	QString szTeacherID; //��ʦ���֤��

	int nTeahcerEducation; //��ʦ�ܽ����̶�
	float fTeacherSalary; //��ʦ����
	QString szTeacherMobile; //��ʦ�ֻ���

	int nTeacherDepartment; //��ʦѧԺ���
	int nTeacherMajor; //��ʦרҵ���
	int nTeacherTitle; //��ʦְ��

	QString szMemo; //��ע
};

class DialogItem : public QDialog
{
	Q_OBJECT

public:
	explicit DialogItem(ItemData* pItemData, QWidget* parent = nullptr);
	~DialogItem();

signals:
	void sigAddItem(ItemData*);

private slots:
	void on_btnAdd_clicked();
	void on_btnCancel_clicked();
	void on_comboBoxDepartment_currentIndexChanged(int Department);
	void on_lineEditJobNumber_TextChanged(const QString& JobNumber);

public:
	static QString GenderToString(int Gender); //�Ա�ת���ַ���
	static QString EducationToString(int Education); //�����̶�ת���ַ���

	static QString DepartmentToString(int Department); //ѧԺת���ַ���	
	static QString MajorToString(int Major); //רҵת���ַ���
	static int StringToMajor(QString StrMajor); //רҵ�ַ���ת�����
	static QString TitleToString(int Title); //ְ��ת���ַ���

private:
	Ui::DialogItem* ui;
	ItemData* mItem;
};
